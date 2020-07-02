//
// Created by liulizhang on 2020/6/17.
//

#include "node.h"
#include <queue>
#include "../lex/CSSLex.h"
std::set<node*> node::update()
{
	std::set<node*> changeNode;
	std::queue<node*> q;
	q.push(this);
	int flag = 1;
	while (!q.empty())
	{
		node* now = q.front();
		q.pop();
		for (auto i:this->_changeSet)
		{
			if (now->vis[i] == 0)
			{
				flag = 1;
				//float *color=this->_style.color;
				std::string vv;
				//now->_final_style.color[0]=color[0];
				//now->_final_style.color[1]=color[1];
				//now->_final_style.color[2]=color[2];
			}
		}
		if (flag)
		{
			changeNode.insert(now);
			for (auto i:now->_children)
			{
				q.push(i);
			}
		}
		flag = 0;
	}
	return changeNode;
}
void node::add_child(node* c)
{
	this->_children.push_back(c);
	c->_parent = this;
}
void node::change(int key, const std::string& value)
{
	vis[key] = true;
	_changeSet.insert(key);
	//_style.set(key,value);
	//_final_style.set(key,value);
}
void node::Render_Width(css_computed_style* style)
{
	css_fixed width = 0;
	css_unit wunit = CSS_UNIT_PX;
	css_width_e wtype = CSS_WIDTH_AUTO;
	wtype = static_cast<css_width_e>(css_computed_width(style, &width, &wunit));
	width = width / 1024;
	box.width = width;
	if (wtype == CSS_WIDTH_AUTO)
	{
		YGNodeStyleSetWidthAuto(ygnode);
	}
	else if (wunit == CSS_UNIT_PCT)
	{
		YGNodeStyleSetWidthPercent(ygnode, width);
	}
	else
	{
		YGNodeStyleSetWidth(ygnode, width);
	}
}
void node::Render_Height(css_computed_style* style)
{
	css_fixed height = 0;
	css_height_e htype = CSS_HEIGHT_AUTO;
	css_unit hunit = CSS_UNIT_PX;
	htype = static_cast<css_height_e>(css_computed_height(style, &height, &hunit));
	height = height / 1024;
	box.height = height;
	if (htype == CSS_HEIGHT_AUTO)
	{
		YGNodeStyleSetHeightAuto(ygnode);
	}
	else if (hunit == CSS_UNIT_PCT)
	{
		YGNodeStyleSetHeightPercent(ygnode, height);
	}
	else
	{
		YGNodeStyleSetHeight(ygnode, height);
	}
}
void node::Render_Direction(css_computed_style* style)
{
	css_direction_e direction_type;
	direction_type = static_cast<css_direction_e>(css_computed_direction(style));
	if (direction_type == CSS_DIRECTION_INHERIT)
	{
		YGNodeStyleSetDirection(ygnode, YGDirectionInherit);
	}
	else if (direction_type == CSS_DIRECTION_LTR)
	{
		YGNodeStyleSetDirection(ygnode, YGDirectionLTR);
	}
	else if (direction_type == CSS_DIRECTION_RTL)
	{
		YGNodeStyleSetDirection(ygnode, YGDirectionRTL);
	}
}
void node::Render_Align_Content(css_computed_style* style)
{
	css_align_content_e align_content_type;
	align_content_type = static_cast<css_align_content_e>(css_computed_align_content(style));
	switch (align_content_type)
	{
	case CSS_ALIGN_CONTENT_FLEX_START:
		YGNodeStyleSetAlignContent(ygnode, YGAlignFlexStart);
		break;
	case CSS_ALIGN_CONTENT_CENTER:
		YGNodeStyleSetAlignContent(ygnode, YGAlignCenter);
		break;
	case CSS_ALIGN_CONTENT_FLEX_END:
		YGNodeStyleSetAlignContent(ygnode, YGAlignFlexEnd);
		break;
	case CSS_ALIGN_CONTENT_STRETCH:
		YGNodeStyleSetAlignContent(ygnode, YGAlignStretch);
		break;
	case CSS_ALIGN_CONTENT_SPACE_BETWEEN:
		YGNodeStyleSetAlignContent(ygnode, YGAlignSpaceBetween);
		break;
	case CSS_ALIGN_CONTENT_SPACE_AROUND:
		YGNodeStyleSetAlignContent(ygnode, YGAlignSpaceAround);
		break;
	case CSS_ALIGN_CONTENT_INHERIT:
		break;
	case CSS_ALIGN_CONTENT_SPACE_EVENLY:
		break;
	}
}

void node::Render_Box_Sizing(css_computed_style* style)
{
	css_box_sizing_e bs = CSS_BOX_SIZING_CONTENT_BOX;
	bs = static_cast<css_box_sizing_e>(css_computed_box_sizing(style));
}
void node::Render_Flex(css_computed_style* style){

}
void node::Render_Flex_Basis(css_computed_style* style)
{
	css_fixed flex_basis_value = 0;
	css_unit flex_basis_unit = CSS_UNIT_PX;
	css_width_e flex_basis_type = CSS_WIDTH_AUTO;
	flex_basis_type = static_cast<css_width_e>(css_computed_flex_basis(style, &flex_basis_value, &flex_basis_unit));
	flex_basis_value/=1024;
	if (flex_basis_type == CSS_WIDTH_AUTO)
	{
		YGNodeStyleSetFlexBasisAuto(ygnode);
	}
	else if (flex_basis_unit == CSS_UNIT_PCT)
	{
		YGNodeStyleSetFlexBasisPercent(ygnode, flex_basis_value);
	}
	else
	{
		YGNodeStyleSetFlexBasis(ygnode, flex_basis_value);
	}
}
void node::Render_Flex_Direction(css_computed_style* style)
{
	css_flex_direction_e flex_direction = static_cast<css_flex_direction_e>(css_computed_flex_direction(style));
	if (flex_direction == CSS_FLEX_DIRECTION_COLUMN)
	{
		YGNodeStyleSetFlexDirection(ygnode, YGFlexDirectionColumn);
	}
	else if (flex_direction == CSS_FLEX_DIRECTION_COLUMN_REVERSE)
	{
		YGNodeStyleSetFlexDirection(ygnode, YGFlexDirectionColumnReverse);
	}
	else if (flex_direction == CSS_FLEX_DIRECTION_ROW)
	{
		YGNodeStyleSetFlexDirection(ygnode, YGFlexDirectionRow);
	}
	else if (flex_direction == CSS_FLEX_DIRECTION_ROW_REVERSE)
	{
		YGNodeStyleSetFlexDirection(ygnode, YGFlexDirectionRowReverse);
	}
}
void node::Render_Flex_Grow(css_computed_style* style)
{
	css_fixed flex_grow;
	css_flex_grow_e flex_grow_type = static_cast<css_flex_grow_e>(css_computed_flex_grow(style, &flex_grow));
	if (flex_grow_type == CSS_FLEX_GROW_SET)
	{
		YGNodeStyleSetFlexGrow(ygnode, flex_grow);
	}
}
void node::Render_Flex_Shrink(css_computed_style* style)
{
	css_fixed flex_shrink;
	css_flex_shrink_e flex_shrink_type = static_cast<css_flex_shrink_e>(css_computed_flex_shrink(style, &flex_shrink));
	if (flex_shrink_type == CSS_FLEX_SHRINK_SET)
	{
		YGNodeStyleSetFlexGrow(ygnode, flex_shrink);
	}
}
void node::Render_Flex_Wrap(css_computed_style* style)
{
	css_flex_wrap_e flex_wrap_type = static_cast<css_flex_wrap_e>(css_computed_flex_wrap(style));
	if (flex_wrap_type == CSS_FLEX_WRAP_NOWRAP)
	{
		YGNodeStyleSetFlexGrow(ygnode, YGWrapNoWrap);
	}
	else if (flex_wrap_type == CSS_FLEX_WRAP_WRAP)
	{
		YGNodeStyleSetFlexGrow(ygnode, YGWrapWrap);
	}
	else if (flex_wrap_type == CSS_FLEX_WRAP_WRAP_REVERSE)
	{
		YGNodeStyleSetFlexGrow(ygnode, YGWrapWrapReverse);
	}
}
void node::Render_Position(css_computed_style* style)
{
	css_fixed left = 0;
	css_unit lunit = CSS_UNIT_PX;
	css_left_e ltype = CSS_LEFT_AUTO;
	ltype = static_cast<css_left_e>(css_computed_left(style, &left, &lunit));
	if (lunit == CSS_UNIT_PCT)
		YGNodeStyleSetPositionPercent(ygnode, YGEdgeLeft, left);
	else
		YGNodeStyleSetPosition(ygnode, YGEdgeLeft, left);

	css_fixed top = 0;
	css_unit tunit = CSS_UNIT_PX;
	css_left_e ttype = CSS_LEFT_AUTO;
	ttype = static_cast<css_left_e>(css_computed_left(style, &top, &tunit));
	if (tunit == CSS_UNIT_PCT)
		YGNodeStyleSetPositionPercent(ygnode, YGEdgeTop, top);
	else
		YGNodeStyleSetPosition(ygnode, YGEdgeTop, top);

	css_fixed right = 0;
	css_unit runit = CSS_UNIT_PX;
	css_left_e rtype = CSS_LEFT_AUTO;
	rtype = static_cast<css_left_e>(css_computed_left(style, &right, &runit));
	if (runit == CSS_UNIT_PCT)
		YGNodeStyleSetPositionPercent(ygnode, YGEdgeRight, right);
	else
		YGNodeStyleSetPosition(ygnode, YGEdgeRight, right);

	css_fixed bottom = 0;
	css_unit bunit = CSS_UNIT_PX;
	css_left_e btype = CSS_LEFT_AUTO;
	btype = static_cast<css_left_e>(css_computed_left(style, &bottom, &bunit));
	if (bunit == CSS_UNIT_PCT)
		YGNodeStyleSetPositionPercent(ygnode, YGEdgeBottom, bottom);
	else
		YGNodeStyleSetPosition(ygnode, YGEdgeBottom, bottom);
}
void node::Render_Position_Type(css_computed_style* style)
{
	css_position_e position_type = static_cast<css_position_e>(css_computed_position(style));
	if (position_type == CSS_POSITION_RELATIVE)
	{
		YGNodeStyleSetFlexGrow(ygnode, YGPositionTypeRelative);
	}
	else if (position_type == CSS_POSITION_ABSOLUTE)
	{
		YGNodeStyleSetFlexGrow(ygnode, YGPositionTypeAbsolute);
	}
}

void node::Render_Display(css_computed_style* style)
{
	box.display = static_cast<css_display_e>(css_computed_display(style, _parent != nullptr));
	if (box.display == CSS_DISPLAY_FLEX)
	{
		YGNodeStyleSetDisplay(ygnode, YGDisplayFlex);
	}
	else if (box.display == CSS_DISPLAY_NONE)
	{
		YGNodeStyleSetDisplay(ygnode, YGDisplayNone);
	}
}
void node::Render_Box(int nWidth, int nHeight, css_pseudo_element mod)
{
	Render_Width(_final_style->styles[mod]);
	Render_Height(_final_style->styles[mod]);
	Render_Box_Sizing(_final_style->styles[mod]);
	Render_Flex(_final_style->styles[mod]);
	Render_Flex_Basis(_final_style->styles[mod]);
	Render_Flex_Direction(_final_style->styles[mod]);
	Render_Flex_Grow(_final_style->styles[mod]);
	Render_Flex_Shrink(_final_style->styles[mod]);
	Render_Position(_final_style->styles[mod]);
	Render_Flex_Wrap(_final_style->styles[mod]);
	Render_Direction(_final_style->styles[mod]);
	Render_Align_Content(_final_style->styles[mod]);
	Render_Position_Type(_final_style->styles[mod]);
	Render_Display(_final_style->styles[mod]);
}
void node::Render_Color(css_pseudo_element mod)
{
	css_color_e color_type;
	css_color color_shade;
	color_type = static_cast<css_color_e>(css_computed_background_color(_final_style->styles[mod], &color_shade));
	char buf[8];
	sprintf(buf, "%x", color_shade);
	for (int i = 0; i < 4; i++)
	{
		uint32_t x;
		sscanf(buf + 2 * i, "%2x", &x);
#ifdef DEBUG_ALL
		printf("%d%c", x,j==3?'\n':',');
#endif
		box.color[i] = x;
	}
}
void node::Render_Time()
{
	css_fixed delay_time = 0;
	css_transition_delay_e delayE;
	css_unit delay_unit = CSS_UNIT_S;
	delayE = static_cast<css_transition_delay_e>(css_computed_transition_delay(_final_style->styles[CSS_PSEUDO_ELEMENT_NONE], &delay_time, &delay_unit));
	css_fixed duration_time = 0;
	css_transition_duration_e durationE;
	css_unit duration_unit = CSS_UNIT_S;
	durationE = static_cast<css_transition_duration_e>(css_computed_transition_duration(_final_style->styles[CSS_PSEUDO_ELEMENT_NONE], &duration_time, &duration_unit));
	delay_time = delay_time * 1000 / 1024;
	duration_time = duration_time * 1000 / 1024;
	box.start_time = delay_time;
	box.end_time = box.start_time + duration_time;
}
