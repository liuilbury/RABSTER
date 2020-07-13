//
// Created by liulizhang on 2020/7/10.
//

#include "Style_Color.h"
void Style_Color::Render_backgroud_color(css_computed_style* style)
{
	css_color_e color_type;
	css_color color_shade;
	color_type = static_cast<css_color_e>(css_computed_background_color(style, &color_shade));
	sprintf(sbackgroud_color.buf, "%x", color_shade);
	sbackgroud_color.calculate_color();
}
void Style_Color::Render_Color(css_computed_style* style)
{
	Render_backgroud_color(style);
}