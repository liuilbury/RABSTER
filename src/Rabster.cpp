//
// Created by liulizhang on 2020/6/29.
//

#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include "html/html.h"
#include "../render/Render.h"
#include <vector>
#include <cmath>
std::wstring utf8_decode(const std::string& str)
{
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}
using namespace Gdiplus;

HWND hwnd;
MSG msg;
WNDCLASS wndclass;
HDC hdc;
HtmlContent* html_ctx;
float nHeight, nWidth;
int flag = 0;
int cnt = 0;
Render* ctx;
Graphics* graphics;

YGSize GetTextBounds(std::string font_name, float emSize, float width, float height, RenderNode* dom)
{
	RectF boundRect[255];
	std::wstring w_font = utf8_decode(font_name);
	FontFamily* fontFamily = new FontFamily(w_font.data());
	Font font(fontFamily, emSize,FontStyleRegular,UnitPixel);
	PointF pointF(0.0f, 0.0f);
	std::wstring w_text;
	for (int i = 0; i <= 255; i++)
	{
		std::string sss;
		sss.push_back(i);
		graphics->MeasureString(utf8_decode(sss).data(), 1, &font, pointF,StringFormat::GenericTypographic(), &boundRect[i]);
		//boundRect[i].Width=floor(boundRect[i].Width);
	}
	int last_pos = 0, last_ws=0;
	YGSize position={0,0}, now_position={0,0};
	std::vector<RenderNode*> children = dom->getChildren();
	for (int i = 0; i < children.size(); i++)
	{
		char c = children[i]->text[0];
		if (c != ' ')
		{
			now_position.width += boundRect[c].Width;
			if (now_position.width > width)
			{
				if (last_pos == 0)
				{
					i--;
				}
				else
				{
					i = last_pos-1;
					last_pos=0;
				}
				now_position.height += font.GetHeight(graphics);
				now_position.width = 0;
			}else{
				position.width=fmax(position.width,now_position.width);
				Box& box = children[i]->style->StyleLayout.getBox();
				box.left = now_position.width-boundRect[c].Width;
				box.top = now_position.height;
			}
		}
		else
		{
			last_pos = i + 1;
			now_position.width += emSize/2;
			if (now_position.width > width)
			{
				now_position.height += font.GetHeight(graphics);
				now_position.width = 0;
				last_pos=0;
			}else{
				position.width=fmax(position.width,now_position.width);
				Box& box = children[i]->style->StyleLayout.getBox();
				box.left= now_position.width-emSize/2;
				box.top  = now_position.height;
			}
		}
	}
	position.height=now_position.height+font.GetHeight(graphics);
	position.width+=dom->style->StyleLayout.sborders.border[0]*2;
	return position;
}
YGSize text_measure(YGNodeRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode)
{
	if (widthMode == YGMeasureModeUndefined)width = 10000;
	if (heightMode == YGMeasureModeUndefined)height = 10000;
	YGSize size = GetTextBounds("Arial", 20, width, height, (RenderNode*)node->getContext());
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
	Box& box = d->style->StyleLayout.getBox();
	if (d->style->StyleLayout.vis)
	{
		left = box.left;
		top = box.top;
		width = box.width;
		height = box.height;
	}
	else
	{
		d->style->StyleLayout.vis = true;
		left = YGNodeLayoutGetLeft(d->ygNode);
		top = YGNodeLayoutGetTop(d->ygNode);
		width = YGNodeLayoutGetWidth(d->ygNode);
		height = YGNodeLayoutGetHeight(d->ygNode);
		if (d->getParent() != nullptr)
		{
			box = d->getParent()->style->StyleLayout.getBox();
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
		left = box.left = left + box.left;
		top = box.top = top + box.top;
		box.width = width;
		box.height = height;
	}
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
	float left, top, width, height;
	get_position(d, left, top, width, height);
	graphics->FillRectangle(solidBrush, left, top, width, height);
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
		FontFamily* fontFamily = new FontFamily(L"Arial");
		Font font(fontFamily, 20,FontStyleRegular,UnitPixel);
		Box& fabox = d->getParent()->style->StyleLayout.getBox();
		std::string s, last;
		YGSize fontSize;
		for(auto i:d->getChildren()){
			Box& box = i->style->StyleLayout.getBox();
			box.left+=fabox.left,box.top+=fabox.top;
			graphics->DrawString(utf8_decode(i->text).data(), -1, &font, PointF(box.left, box.top), solidBrush);
			//printf("%s\n%f %f %f %f\n", i->get_Name().data(), box.left, box.top, box.width, box.height);
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
	if (d->element != nullptr)
	{
		print_border(d, graphics);
		print_string(d, graphics);
		print_time(d, graphics);
	}
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
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		return 0;
	case WM_SIZE:
	{
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
		GetClientRect(hwnd, &rctA);
		nWidth = rctA.right - rctA.left;   //窗口的宽度
		nHeight = rctA.bottom - rctA.top;  //窗口的高度
		PAINTSTRUCT ps;
		hdc = BeginPaint(hwnd, &ps);
		graphics = new Graphics(hdc);
		ctx->Clean_Tree();
		ctx->Render_Tree();
		ctx->Layout_Tree();
		YGNodeStyleSetMaxWidth(ctx->root->ygNode, nWidth);
		YGNodeStyleSetMaxHeight(ctx->root->ygNode, nHeight);
		YGNodeCalculateLayout(ctx->root->ygNode, YGUndefined, YGUndefined, YGDirectionLTR);
		show_tree(ctx->root);
		EndPaint(hwnd, &ps);
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	html_init();
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	static TCHAR szAppName[] = TEXT("Rabster");
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;
	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("This program requires Window NT!"), szAppName, MB_ICONERROR);
		return 0;
	}
	hwnd = CreateWindow(szAppName,
		TEXT("Rabster"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, nullptr, 0, false))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	return msg.wParam;
}
