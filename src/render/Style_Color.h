//
// Created by liulizhang on 2020/7/10.
//

#ifndef _STYLE_COLOR_H_
#define _STYLE_COLOR_H_
#include <libcss/libcss.h>
#include <cstdio>
struct SColor{
 public:
	char buf[8];
	int color[4];
	void calculate_color(){
		for (int i = 0; i < 4; i++)
		{
			uint32_t x;
			sscanf(buf + 2 * i, "%2x", &x);
#ifdef DEBUG_ALL
			printf("%d%c", x,j==3?'\n':',');
#endif
			color[i] = x;
		}
	}
};
class Style_Color
{
 private:
	void Render_backgroud_color(css_computed_style* style);
 public:
	SColor sbackgroud_color;
	SColor color;
	void Render_Color(css_computed_style* style);
};

#endif //_STYLE_COLOR_H_
