//
// Created by liulizhang on 2020/7/10.
//

#include "Style_Color.h"
void Style_Color::Render_background_color(css_computed_style* style)
{
	css_color_e color_type;
	css_color color_shade;
	color_type = static_cast<css_color_e>(css_computed_background_color(style, &color_shade));
	sprintf(sbackground_color.buf, "%x", color_shade);
	sbackground_color.calculate_color();
}
void Style_Color::Render_border_color(css_computed_style* style)
{
	css_color_e color_type;
	css_color color_shade;
	color_type = static_cast<css_color_e>(css_computed_border_left_color(style, &color_shade));
	sprintf(sborder_color[0].buf, "%x", color_shade);
	sborder_color[0].calculate_color();
	color_type = static_cast<css_color_e>(css_computed_border_top_color(style, &color_shade));
	sprintf(sborder_color[1].buf, "%x", color_shade);
	sborder_color[1].calculate_color();
	color_type = static_cast<css_color_e>(css_computed_border_right_color(style, &color_shade));
	sprintf(sborder_color[2].buf, "%x", color_shade);
	sborder_color[2].calculate_color();
	color_type = static_cast<css_color_e>(css_computed_border_bottom_color(style, &color_shade));
	sprintf(sborder_color[3].buf, "%x", color_shade);
	sborder_color[3].calculate_color();
}
void Style_Color::Render_color(css_computed_style* style)
{
	css_color_e color_type;
	css_color color_shade;
	color_type = static_cast<css_color_e>(css_computed_color(style, &color_shade));
	sprintf(scolor.buf, "%x", color_shade);
	scolor.calculate_color();
}
void Style_Color::Render_Color(css_computed_style* style)
{
	Render_background_color(style);
	Render_border_color(style);
	Render_color(style);
}