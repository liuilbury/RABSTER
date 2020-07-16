//
// Created by liulizhang on 2020/6/29.
//

#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include "html/html.h"
#include "../render/Render.h"
#include <vector>

std::wstring utf8_decode(const std::string& str)
{
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}
using namespace Gdiplus;

HWND hwnd;    //创建窗口函数CreateWindow()会返回一个HWND句柄，这里定义下，用来接收这个句柄
MSG msg;      //消息结构体，在消息循环的时候需要
WNDCLASS wndclass; //创建窗口类对象
HDC hdc;
HtmlContent* html_ctx;
float nHeight, nWidth;
int flag = 0;
int cnt = 0;
Render* ctx;
Graphics* graphics;

YGSize GetTextBounds(std::string font_name, float emSize, float width, float height,std::string text)
{
	std::wstring w_font = utf8_decode(font_name);
	Font font(w_font.data(), emSize);
	std::wstring w_text = utf8_decode(text);
	RectF layoutRect(0, 0, width, height);
	StringFormat stringformat(StringAlignmentNear);
	RectF stringRect;
	graphics->MeasureString(w_text.data(), (int)wcslen(w_text.data()), &font, layoutRect, &stringformat, &stringRect);
	return {
		.width=stringRect.Width,
		.height=stringRect.Height
	};
}
YGSize text_measure(YGNodeRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
{
	std::string text = ((RenderNode*)node->getContext())->text;
	if (widthMode == YGMeasureModeUndefined)width = 10000;
	if (heightMode == YGMeasureModeUndefined)height = 10000;
	YGSize size = GetTextBounds("Arial", 20, width, height, text);
	width = size.width;
	height = size.height;
	return {
		.width=width,
		.height=height
	};
}
void html_init()
{
	html_ctx = new HtmlContent();
	html_ctx->html_init(R"(F:\opengl\RABSTER\resources\test.html)");
	ctx = new Render();
	ctx->measure = text_measure;
	ctx->root = ctx->build(html_ctx->root, nullptr);
	ctx->Render_Tree();
	ctx->Layout_Tree();
	//ctx->Print_Tree();
}
void html_destroy(DomNode* d)
{
	for (auto i:d->_children)
	{
		html_destroy(i);
	}
}
void get_position(RenderNode* d, float& left, float& top, float& width, float& height)
{
	left = YGNodeLayoutGetLeft(d->ygNode);
	top = YGNodeLayoutGetTop(d->ygNode);
	width = YGNodeLayoutGetWidth(d->ygNode);
	height = YGNodeLayoutGetHeight(d->ygNode);
}
void print_position(RenderNode* d, Graphics* graphics)
{
	int background_color[4];
	for (int i = 0; i < 4; i++)
	{
		background_color[i] = d->style->StyleColor.sbackground_color.color[i];
	}
	SolidBrush* solidBrush = new SolidBrush(Color(background_color[0], background_color[1],
		background_color[2] + cnt, background_color[3]));
	Box& box = d->style->StyleLayout.getBox();
	if (d->getParent() != nullptr)
	{
		box=d->getParent()->style->StyleLayout.getBox();
		if (d->getParent()->style->StyleLayout.sdisplay == 2)
		{
			if (d->getPrev() != nullptr)
			{
				Box& br_box = d->getPrev()->style->StyleLayout.getBox();
				box.top = br_box.top + br_box.height;
			}
		}
		else
		{
			Box& fa_box = d->getParent()->style->StyleLayout.getBox();
			box = fa_box;
		}
	}
	else
	{
		box.left = box.top = 0;
	}
	float left, top, width, height;
	get_position(d, left, top, width, height);
	box.left += left, box.top += top;
	box.width = width, box.height = height;
	graphics->FillRectangle(solidBrush, box.left, box.top, width, height);
#ifdef DEBUG
	printf("%s ", d->get_Name().data());
	printf("%f %f\n",left,top);
	printf("%f %f %f %f\n", box.left, box.top, width, height);
#endif
}
void print_border(RenderNode* d, Graphics* graphics)
{
	float left, top, width, height;
	get_position(d, left, top, width, height);
	float cleft, ctop, cright, cbottom;
	cleft = YGNodeStyleGetBorder(d->ygNode, YGEdgeLeft);
	ctop = YGNodeStyleGetBorder(d->ygNode, YGEdgeTop);
	cright = YGNodeStyleGetBorder(d->ygNode, YGEdgeRight);
	cbottom = YGNodeStyleGetBorder(d->ygNode, YGEdgeBottom);
	int border_color[4][4];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			border_color[i][j] = d->style->StyleColor.sborder_color[i].color[j];
		}
	}
	Box& box = d->style->StyleLayout.getBox();
	Pen* pen = new Pen(Color(0, 0, 0, 0), 0);
	pen->SetColor(Color(border_color[0][0], border_color[0][1], border_color[0][2], border_color[0][3]));
	pen->SetWidth(cleft);
	pen->SetAlignment(PenAlignmentInset);
	graphics->DrawRectangle(pen, box.left, box.top, width, height);
}
void print_string(RenderNode* d, Graphics* graphics)
{
	if (!d->text.empty())
	{
		int color[4];
		for (int i = 0; i < 4; i++)
		{
			color[i] = d->style->StyleColor.scolor.color[i];
		}
		SolidBrush* solidBrush = new SolidBrush(Color(255, color[1], color[2], color[3]));
		FontFamily fontFamily(L"Arial");
		Font font(&fontFamily, 20, FontStyleRegular, UnitPixel);
		Box& box = d->style->StyleLayout.getBox();
		RectF rectF(box.left, box.top, box.width, box.height);
		std::string s,last;
		YGSize fontSize;
		float now_top=box.top;
		int count=0;
		for(int i0=0;i0<d->data.size();i0++){
			char* i=d->data[i0];
			if(count!=0)s+=' ';
			s+=i;
			count++;
			fontSize=GetTextBounds("Arial",20,10000,10000,s);
			if(fontSize.width>box.width){
				if(count>1)
				{
					std::wstring data = utf8_decode(last);
					graphics->DrawString(data.c_str(), -1, &font, PointF(box.left, now_top), solidBrush);
					now_top += fontSize.height;
					s = "";
					last="";
					i0--;
					count=0;
				}else{
					std::string ss="";
					last="";
					for(int j=0;j<s.size();j++){
						ss+=s[j];
						fontSize=GetTextBounds("Arial",20,10000,10000,ss);
						if(fontSize.width>box.width){
							std::wstring data = utf8_decode(last);
							graphics->DrawString(data.c_str(), -1, &font, PointF(box.left, now_top), solidBrush);
							now_top += fontSize.height;
							ss="";
							last="";
							j--;
							count=0;
						}
						else
						{
							last += s[j];
						}
					}
					if(!last.empty()){
						count=1;
					}
				}
			}else{
				last=s;
			}
		}
		if(!last.empty()){
			std::wstring data = utf8_decode(last);
			graphics->DrawString(data.c_str(), -1, &font, PointF(box.left, now_top), solidBrush);
			last="";
		}
	}
}
void print_time(RenderNode* d, Graphics* graphics)
{
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
}
void show_node(RenderNode* d)
{
	print_position(d, graphics);
	print_border(d, graphics);
	print_string(d, graphics);
	print_time(d, graphics);
}
void show_tree(RenderNode* d)
{
	show_node(d);
	for (auto i:d->getChildren())
	{
		show_tree(i);
	}
}
void test()
{
	int cc = 0;
	DomNode* d = html_ctx->root;
	std::queue<DomNode*> q;
	q.push(d);
	while (!q.empty())
	{
		d = q.front();
		q.pop();
		if (d->real_name == "div" && d->_next != nullptr)
		{
			cc++;
			if (cc >= 3)
			{
				d->link = d->_next;
			}
		}
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
		return 0;
	}
	case WM_TIMER:
	{
		if (!flag)
		{
			KillTimer(hwnd, 1);
			return 0;
		}
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
			InvalidateRect(hwnd, &rctA, true);
		}
		else if (wParam == VK_F2)
		{
			//Save_Style(html_ctx->root);
			flag = 1;
			html_ctx->root->hover = true;
			html_ctx->get_tree_style(html_ctx->root);
			SetTimer(hwnd, 1, 16, NULL);
		}
		else if (wParam == VK_F3)
		{
			test();
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
		graphics = new Graphics(hdc);
		ctx->Render_Tree();
		ctx->Layout_Tree();
		YGNodeStyleSetMaxWidth(ctx->root->ygNode,nWidth);
		YGNodeStyleSetMaxHeight(ctx->root->ygNode,nHeight);
		YGNodeCalculateLayout(ctx->root->ygNode, YGUndefined, YGUndefined, YGDirectionLTR);
		show_tree(ctx->root);
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
