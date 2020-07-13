//
// Created by liulizhang on 2020/7/10.
//

#ifndef _STYLE_H_
#define _STYLE_H_
#include "Style_Color.h"
#include "Style_Layout.h"
#include "Yoga.h"
#include "YGStyle.h"
#include "YGLayout.h"
#include "YGNode.h"
class Style
{
	Style_Layout StyleLayout;
 public:
	Style_Color StyleColor;
	void Render_Style(css_computed_style* style,bool);
	Style_Layout* get_Layout();
	void print();
};

#endif //_STYLE_H_
