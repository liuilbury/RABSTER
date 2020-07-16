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
			color[i] = x;
		}
	}
	void print(){
		for(int i=0;i<4;i++){
			printf("%d%c",color[i],i==3?'\n':',');
		}
	}
};
class Style_Color
{
 private:
	void Render_background_color(css_computed_style* style);
	void Render_border_color(css_computed_style* style);
	void Render_color(css_computed_style* style);
 public:
	SColor sbackground_color;
	SColor sborder_color[4];
	SColor scolor;
	void Render_Color(css_computed_style* style);
};

#endif //_STYLE_COLOR_H_
