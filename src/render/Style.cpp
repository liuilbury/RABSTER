//
// Created by liulizhang on 2020/7/10.
//

#include "Style.h"
void Style::Render_Style(css_computed_style* style,bool hasparent)
{
	StyleLayout.Render_Layout(style,hasparent);
	StyleColor.Render_Color(style);
}
Style_Layout* Style::get_Layout()
{
	return &StyleLayout;
}

