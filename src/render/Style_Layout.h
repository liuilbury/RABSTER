//
// Created by liulizhang on 2020/7/10.
//
#include <libcss/libcss.h>
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
};
struct SPosition
{
	SValue position[4];
	SPositionType type;
};
struct SMargin{
	SValue margin[4];
};
struct SBorder{
	SValue border[4];
};
struct SPadding{
	SValue padding[4];
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
};

#endif //_STYLE_LAYOUT_H_
