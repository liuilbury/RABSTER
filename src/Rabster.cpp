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
void html_init()
{
	root = html_init(R"(F:\opengl\glfw\test.json)");
	html_ctx = new HtmlContent();
	html_ctx->html_css_new_stylesheets();
	html_ctx->html_css_append_stylesheets(R"(F:\opengl\glfw\test.css)");
	html_ctx->html_css_new_selection_context();
	html_ctx->get_tree_style(root);
}
void html_destroy(node *d){
	for(auto i:d->_children){
		html_destroy(d);
	}
}
int cnt=0;
void Render_Node(node* d)
{
	d->Render_Box(nWidth,nHeight);
	d->Render_Color();
	d->Render_display();
	Graphics* graphics = new Graphics(hdc);
	//Pen* pen=new Pen(Color(255,255,0,0),3);
	SolidBrush* solidBrush = new SolidBrush(Color(d->box.color[0], d->box.color[1], d->box.color[2]+cnt, d->box.color[3]));
	//printf("%d %d %d %d\n",d->box.color[0],d->box.color[1],d->box.color[2]+cnt,d->box.color[3]);
	//graphics->DrawRectangle(pen,20,10,200,100);
	int nowx=0,nowy=0;
	if(d->_prev!= nullptr){
		nowy=d->_prev->box.y;
	}
	graphics->FillRectangle(solidBrush, nowx, nowy, d->box.width, d->box.height);
	d->box.y=nowy+d->box.height;
	//printf("%s %d %d %d %d\n",d->real_name.c_str(),nowx,nowy,d->box.width,d->box.height);
}
void Render_Tree(node* d)
{
	std::queue<node*>q;
	q.push(d);
	while(!q.empty()){
		d=q.front();
		q.pop();
		Render_Node(d);
		for (auto i:d->_children)
		{
			q.push(i);
		}
	}
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
		printf("11\n");
		return 0;
	}
	case WM_KEYDOWN:
	{
		if (wParam == VK_F1)
		{
			cnt=cnt+10;
			//html_init();
			RECT rctA;
			rctA.right=nWidth;
			rctA.bottom=nHeight;
			rctA.left=rctA.top=0;
			InvalidateRect(hwnd,&rctA, false);
		}
		return 0;
	}
	case WM_PAINT:
	{
		RECT rctA;
		GetWindowRect(hwnd, &rctA);
		printf("%d %d %d %d\n",rctA.right,rctA.left,rctA.bottom,rctA.top);

		nWidth = rctA.right - rctA.left;   //窗口的宽度
		nHeight = rctA.bottom - rctA.top;  //窗口的高度
		PAINTSTRUCT ps;
		hdc = BeginPaint(hwnd, &ps);
		Render_Tree(root);
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
