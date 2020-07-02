//
// Created by liulizhang on 2020/6/29.
//

#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include "html/html.h"
#include <vector>
using namespace Gdiplus;
HWND hwnd;    //创建窗口函数CreateWindow()会返回一个HWND句柄，这里定义下，用来接收这个句柄
MSG msg;      //消息结构体，在消息循环的时候需要
WNDCLASS wndclass; //创建窗口类对象
HDC hdc;
node* root;
HtmlContent* html_ctx;
int nHeight, nWidth;
int flag = 0;
int time = 0, cnt = 0;
void html_init()
{
	root = html_init(R"(F:\opengl\RABSTER\test.html)");
	html_ctx = new HtmlContent();
	html_ctx->html_css_new_stylesheets();
	for(auto i:root->url)
	{
		std::string str=R"(F:\opengl\RABSTER\)"+i;
		html_ctx->html_css_append_stylesheets(str.data());
	}
	html_ctx->html_css_new_selection_context();
	html_ctx->get_tree_style(root);
}
void html_destroy(node* d)
{
	for (auto i:d->_children)
	{
		html_destroy(d);
	}
}
void Render_Node(node* d)
{
	css_pseudo_element mod = CSS_PSEUDO_ELEMENT_NONE;
	d->Render_Box(nWidth, nHeight, mod);
	d->Render_Color(mod);
	d->Render_Time();
}
void Render_Tree(node* d)
{
	flag = 0;
	std::queue<node*> q;
	q.push(d);
	while (!q.empty())
	{
		d = q.front();
		q.pop();
		Render_Node(d);
		for (auto i:d->_children)
		{
			q.push(i);
		}
	}
}
void Save_Node(node* d)
{
	d->save_style = d->box;
}
void Save_Style(node* d)
{
	Save_Node(d);
	for (auto i:d->_children)
	{
		Save_Style(i);
	}
}
void show_node(node *d){
	/*if (time != 0&&d->box.end_time!=0)
	{
		if (d->box.start_time <= time && d->box.end_time > time)
		{
			flag = 1;
			int all_time = d->box.end_time - d->box.start_time;
			int spend_time = time - d->box.start_time;
			d->box.color[0] = d->save_style.color[0]+(d->box.color[0] - d->save_style.color[0]) * spend_time / all_time;
			d->box.color[1] = d->save_style.color[1]+(d->box.color[1] - d->save_style.color[1]) * spend_time / all_time;
			d->box.color[2] = d->save_style.color[2]+(d->box.color[2] - d->save_style.color[2]) * spend_time / all_time;
			d->box.color[3] = d->save_style.color[3]+(d->box.color[3] - d->save_style.color[3]) * spend_time / all_time;
			d->box.width = d->save_style.width+(d->box.width - d->save_style.width) * spend_time / all_time;
			//d->box.height = d->save_style.width+(d->box.height - d->save_style.height) * spend_time / all_time;
		}
	}*/
	Graphics* graphics = new Graphics(hdc);
	SolidBrush* solidBrush = new SolidBrush(Color(d->box.color[0], d->box.color[1],d->box.color[2] + cnt, d->box.color[3]));
	float left,top,width,height;
	left=YGNodeLayoutGetLeft(d->ygnode);
	top=YGNodeLayoutGetTop(d->ygnode);
	width=YGNodeLayoutGetWidth(d->ygnode);
	height=YGNodeLayoutGetHeight(d->ygnode);
	printf("%s ",d->real_name.data());
	printf("%f %f %f %f\n",left,top,width,height);
	printf("%d %d\n",d->box.width,d->box.height);
	graphics->FillRectangle(solidBrush,left,top,width,height);
}
void show_tree(node* d){
	show_node(d);
	for(auto i:d->_children){
		show_tree(i);
	}
#ifdef DEBUG
	d->box.to_string();
#endif
}
/*窗口回调函数*/
LRESULT CALLBACK WndProc(HWND hwnd, UINT message/*窗口消息*/, WPARAM wParam, LPARAM lParam)
{
	//处理消息
	switch (message)
	{
	case WM_CREATE:
		/*窗口在创建时，会接收到该消息，通常在这里进行初始化操作*/

		return 0;
	case WM_SIZE:
	{
		/*窗口被改变大小时，会接收到该消息，在窗口被创建时也会收到一次*/
		return 0;
	}
	case WM_MOUSEHOVER:
	{
		return 0;
	}
	case WM_TIMER:
	{
		if (!flag)
		{
			KillTimer(hwnd, 1);
			return 0;
		}
		time += 16;
		RECT rctA;
		rctA.right = nWidth;
		rctA.bottom = nHeight;
		rctA.left = rctA.top = 0;
		InvalidateRect(hwnd, &rctA, false);
		return 0;
	}
	case WM_KEYDOWN:
	{
		if (wParam == VK_F1)
		{
			cnt += 10;
			//html_init();
			RECT rctA;
			rctA.right = nWidth;
			rctA.bottom = nHeight;
			rctA.left = rctA.top = 0;
			InvalidateRect(hwnd, &rctA, false);
		}
		else if (wParam == VK_F2)
		{
			Save_Style(root);
			flag = 1;
			root->hover = true;
			html_ctx->get_tree_style(root);
			SetTimer(hwnd, 1, 16, NULL);
		}
		return 0;
	}
	case WM_PAINT:
	{
		RECT rctA;
		GetWindowRect(hwnd, &rctA);
		nWidth = rctA.right - rctA.left;   //窗口的宽度
		nHeight = rctA.bottom - rctA.top;  //窗口的高度
		PAINTSTRUCT ps;
		hdc = BeginPaint(hwnd, &ps);
		Render_Tree(root);
		YGNodeCalculateLayout(root->ygnode,nWidth,nHeight,YGDirectionLTR);
		show_tree(root);
		EndPaint(hwnd, &ps);
		/*窗口有绘图操作更新时,会收到这个消息*/
		return 0;
	}
	case WM_DESTROY:
		/*关闭窗口时，会收到该消息，PostQuitMessage()像系统表明终止当前线程，没有这个函数的话，窗口不会关闭*/
		PostQuitMessage(0);
		return 0;
	}
	//将不需要处理的消息传递给系统作默认处理
	return DefWindowProc(hwnd, message, wParam, lParam);
}/*
    函数名：WinMain:入口函数
    参数：
         hInstance是windows传递的窗口句柄
         hPrevInstance是这个窗口的前一个窗口的句柄，目前没有用，只是为了兼容以前的版本
         szCmdLine是传递进来的命令行命令，这里没有使用
         iCmdShow指定的窗口的显示方式如：显示，隐藏等。
*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	html_init();
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	//初始化gdi+
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	static TCHAR szAppName[] = TEXT("Rabster"); //创建窗口的时候需要一个窗口类名


	//对窗口类的各属性进行初始化
	wndclass.style = CS_HREDRAW | CS_VREDRAW; /*窗口类的风格，CS前缀,C表示Class,S表示
                    Style，这里使用了水平和垂直风格*/
	wndclass.lpfnWndProc = WndProc;  /*这里将回到函数的名字赋值用以windows后面回调*/
	wndclass.cbClsExtra = 0;  //附加参数，通常情况下为0
	wndclass.cbWndExtra = 0;  //附加参数，通常情况下为0
	wndclass.hInstance = hInstance;  //窗口句柄，这里将WinMain中的hInstance句柄赋值就可
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION); /*窗口图标，LoadIcon()是加载图标，这里是加载一个系统资源图标，LoadIcon()的原型是HICON LoadIcon(HINSTANCE, LPCSTR);*/
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);  /*加载鼠标，同上相似*/
	wndclass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);  /*窗口画刷，这里是使用的白色画刷，所以创建出来的窗口的背景颜色则是白色的*/
	wndclass.lpszMenuName = NULL;  //窗口菜单名称，这里没有菜单，设为NULL
	wndclass.lpszClassName = szAppName;  //窗口类名称，这个窗口类名称可作为这个窗口的唯一标识

	/*注册窗口类*/
	if (!RegisterClass(&wndclass))
	{
		//注册窗口类失败时，弹出提示
		MessageBox(NULL, TEXT("This program requires Window NT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	/*创建窗口
	  HWND CreateWindow(
		LPCTSTR lpClassName,  //窗口类名
		LPCTSTR lpWindowName, //窗口标题
		DWORD dwStyle,        //窗口风格，WS开头
		int x,                //窗口左上角x坐标
		int y,                //窗口左上角y坐标
		int nWidth,           //窗口宽度
		int nHeight,          //窗口高度
		HWND hWndParent,      //父窗口句柄
		HMENU hMenu,          //菜单句柄
		HANDLE hlnstance,     //窗口句柄
		LPVOID lpParam);      //改值会传递给窗口WM_CREATE消息的一个参数
	*/
	hwnd = CreateWindow(szAppName,    //窗口类名
		TEXT("Rabster"),    //窗口标题，会在窗口的左上角标题栏显示
		WS_OVERLAPPEDWINDOW, //窗口风格
		CW_USEDEFAULT,  //窗口左上角x位置，这里使用的系统默认值，可自定义
		CW_USEDEFAULT,  //窗口左上角y位置
		CW_USEDEFAULT,  //窗口的宽度
		CW_USEDEFAULT,  //窗口的高度
		NULL, //该窗口的父窗口或所有者窗口的句柄，这里用不到，设为NULL
		NULL, //窗口菜单句柄，这里没有菜单，设置为NULL
		hInstance, //窗口句柄
		NULL  //传递给窗口WM_CREATE消息的一个参数，这里不用，设置为NULL
	);

	/*显示窗口，显示方式使用WinMain的参数*/
	ShowWindow(hwnd, iCmdShow);
	/*更新窗口*/
	UpdateWindow(hwnd);
	/*消息循环，获取消息*/
	while (GetMessage(&msg, nullptr, 0, false))
	{
		//翻译消息
		TranslateMessage(&msg);
		//派发消息
		DispatchMessage(&msg);
	}
	//返回消息的wParam参数给系统
	return msg.wParam;
}
