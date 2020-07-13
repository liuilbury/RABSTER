//
// Created by liulizhang on 2020/7/10.
//
#include <libcss/libcss.h>
#include <cstdio>
#ifndef _STYLE_LAYOUT_H_
#define _STYLE_LAYOUT_H_
enum SUnit
{
	SUnitUndefined,
	SUnitPoint,
	SUnitPercent,
	SUnitAuto
};
enum SOverflow
{
	SOverflowVisible,
	SOverflowHidden,
	SOverflowScroll
};
enum SAlign
{
	SAlignAuto,
	SAlignFlexStart,
	SAlignCenter,
	SAlignFlexEnd,
	SAlignStretch,
	SAlignBaseline,
	SAlignSpaceBetween,
	SAlignSpaceAround
};
enum SDisplay
{
	SDisplayFlex,
	SDisplayNone
};
enum SWrap
{
	SWrapNoWrap,
	SWrapWrap,
	SWrapWrapReverse
};
enum SJustify
{
	SJustifyFlexStart,
	SJustifyCenter,
	SJustifyFlexEnd,
	SJustifySpaceBetween,
	SJustifySpaceAround,
	SJustifySpaceEvenly
};
enum SPositionType
{
	SPositionTypeRelative,
	SPositionTypeAbsolute
};
enum SBorderType{
	SBorderTypeInherit,
	SBorderTypeNone	,
	SBorderTypeHidden,
	SBorderType_Dotted,
	SBorderTypeDashed,
	SBorderTypeSolid,
	SBorderTypeDouble,
	SBorderTypeGroove,
	SBorderType_Rider,
	SBorderTypeInset,
	SBorderTypeOutset
};
enum SEdge
{
	SEdgeLeft,
	SEdgeTop,
	SEdgeRight,
	SEdgeBottom,
	SEdgeStart,
	SEdgeEnd,
	SEdgeHorizontal,
	SEdgeVertical,
	SEdgeAll
};
struct SValue
{
	float value;
	SUnit unit;
	void print(){
		printf("%f--",value);
		switch (unit)
		{

		case SUnitUndefined:
			printf("defined ");
			break;
		case SUnitPoint:
			printf("point ");
			break;
		case SUnitPercent:
			printf("percent ");
			break;
		case SUnitAuto:
			printf("auto ");
			break;
		}
	}
};
struct SPosition
{
	SValue position[4];
	SPositionType type;
	void print(){
		printf("type:");
		switch (type)
		{

		case SPositionTypeRelative:
			printf("relative");
			break;
		case SPositionTypeAbsolute:
			printf("absolute");
			break;
		}
		printf("|||");
		for(int i=0;i<4;i++)
			position[i].print();

	}
};
struct SMargin{
	SValue margin[4];
	void print(){
		for(int i=0;i<4;i++)
			margin[i].print();
	}
};
struct SBorder{
	float border[4];
	SBorderType type[4];
	void print(){
		for(int i=0;i<4;i++)
		{
			printf("%f-- ", border[i]);
			switch (type[i])
			{
			case SBorderTypeInherit:
				printf("inherit ");
				break;
			case SBorderTypeNone:
				printf("none ");
				break;
			case SBorderTypeHidden:
				printf("hidden ");
				break;
			case SBorderType_Dotted:
				printf("dotted ");
				break;
			case SBorderTypeDashed:
				printf("dashed ");
				break;
			case SBorderTypeSolid:
				printf("solid ");
				break;
			case SBorderTypeDouble:
				printf("double ");
				break;
			case SBorderTypeGroove:
				printf("groove ");
				break;
			case SBorderType_Rider:
				printf("rider ");
				break;
			case SBorderTypeInset:
				printf("inset ");
				break;
			case SBorderTypeOutset:
				printf("outset ");
				break;
			}
		}
	}
};
struct SPadding{
	SValue padding[4];
	void print(){
		for(int i=0;i<4;i++)
			padding[i].print();
	}
};
class Style_Layout
{
 public:
	SValue swidth;
	SValue sheight;
	SAlign salignContent;
	SAlign salignItems;
	SAlign salignSelf;
	SDisplay sdisplay;
	SWrap sflex_wrap;
	float sflex_grow;
	float sflex_shrink;
	SValue sflex_basis;
	SJustify sjustify;
	SPosition spositions;
	SMargin smargins;
	SBorder sborders;
	SPadding spaddings;
	SValue smin_width;
	SValue smin_height;
	SValue smax_width;
	SValue smax_height;
	void Render_width(css_computed_style* style);
	void Render_height(css_computed_style* style);
	void Render_alignContent(css_computed_style* style);
	void Render_alignItems(css_computed_style* style);
	void Render_alignSelf(css_computed_style* style);
	void Render_display(css_computed_style* style,bool);
	void Render_flex_wrap(css_computed_style* style);
	void Render_flex_grow(css_computed_style* style);
	void Render_flex_shrink(css_computed_style* style);
	void Render_flex_basis(css_computed_style* style);
	void Render_justify(css_computed_style* style);
	void Render_positions(css_computed_style* style);
	void Render_margins(css_computed_style* style);
	void Render_borders(css_computed_style* style);
	void Render_paddings(css_computed_style* style);
	void Render_min_width(css_computed_style* style);
	void Render_min_height(css_computed_style* style);
	void Render_max_width(css_computed_style* style);
	void Render_max_height(css_computed_style* style);
	void Render_Layout(css_computed_style* style,bool);
	void print();
};

#endif //_STYLE_LAYOUT_H_
