//
// Created by liulizhang on 2020/7/10.
//

#include "Style_Layout.h"
enum value_type
{
	width,
	height,
	position_left,
	position_top,
	position_right,
	position_bottom,
	border_left,
	border_top,
	border_right,
	border_bottom,
	margin_left,
	margin_top,
	margin_right,
	margin_bottom,
	padding_left,
	padding_top,
	padding_right,
	padding_bottom,
	min_width,
	min_height,
	max_width,
	max_height
};
void setValue(css_computed_style* style, value_type t, css_fixed& value, css_unit& unit, int& type)
{
	switch (t)
	{
	case width:
		type = css_computed_width(style, &value, &unit);
		break;
	case height:
		type = css_computed_height(style, &value, &unit);
		break;
	case position_left:
		type = css_computed_left(style, &value, &unit);
		break;
	case position_top:
		type = css_computed_top(style, &value, &unit);
		break;
	case position_right:
		type = css_computed_right(style, &value, &unit);
		break;
	case position_bottom:
		type = css_computed_bottom(style, &value, &unit);
		break;
	case border_left:
		type = css_computed_border_left_width(style, &value, &unit);
		break;
	case border_top:
		type = css_computed_border_top_width(style, &value, &unit);
		break;
	case border_right:
		type = css_computed_border_right_width(style, &value, &unit);
		break;
	case border_bottom:
		type = css_computed_border_bottom_width(style, &value, &unit);
		break;
	case margin_left:
		type = css_computed_margin_left(style, &value, &unit);
		break;
	case margin_top:
		type = css_computed_margin_top(style, &value, &unit);
		break;
	case margin_right:
		type = css_computed_margin_right(style, &value, &unit);
		break;
	case margin_bottom:
		type = css_computed_margin_bottom(style, &value, &unit);
		break;
	case padding_left:
		type = css_computed_padding_left(style, &value, &unit);
		break;
	case padding_top:
		type = css_computed_padding_top(style, &value, &unit);
		break;
	case padding_right:
		type = css_computed_padding_right(style, &value, &unit);
		break;
	case padding_bottom:
		type = css_computed_padding_bottom(style, &value, &unit);
		break;
	case min_width:
		type = css_computed_min_width(style, &value, &unit);
		break;
	case min_height:
		type = css_computed_min_height(style, &value, &unit);
		break;
	case max_width:
		type = css_computed_max_width(style, &value, &unit);
		break;
	case max_height:
		type = css_computed_max_height(style, &value, &unit);
		break;
	}
	value /= 1024;
}
void Style_Layout::Render_Layout(css_computed_style* style, bool hasparent)
{
	Render_width(style);
	Render_height(style);
	Render_alignContent(style);
	Render_alignItems(style);
	Render_alignSelf(style);
	Render_display(style, hasparent);
	Render_flex_wrap(style);
	Render_flex_grow(style);
	Render_flex_shrink(style);
	Render_flex_basis(style);
	Render_flex_direction(style);
	Render_justify(style);
	Render_positions(style);
	Render_margins(style);
	Render_borders(style);
	Render_paddings(style);
	Render_min_width(style);
	Render_min_height(style);
	Render_max_width(style);
	Render_max_height(style);
}
void Style_Layout::Render_width(css_computed_style* style)
{
	css_fixed width = 0;
	css_unit wunit = CSS_UNIT_PX;
	css_width_e wtype = CSS_WIDTH_AUTO;
	wtype = static_cast<css_width_e>(css_computed_width(style, &width, &wunit));
	width = width / 1024;
	swidth.value = width;
	if (wtype == CSS_WIDTH_AUTO)
	{
		swidth.unit = SUnitAuto;
	}
	else if (wtype == CSS_WIDTH_SET)
	{
		if (wunit == CSS_UNIT_PCT)
		{
			swidth.unit = SUnitPercent;
		}
		else
		{
			swidth.unit = SUnitPoint;
		}
	}
	else
	{
		swidth.unit = SUnitUndefined;
	}
}
void Style_Layout::Render_height(css_computed_style* style)
{
	css_fixed height = 0;
	css_height_e htype = CSS_HEIGHT_AUTO;
	css_unit hunit = CSS_UNIT_PX;
	htype = static_cast<css_height_e>(css_computed_height(style, &height, &hunit));
	height = height / 1024;
	sheight.value=height;
	if (htype == CSS_HEIGHT_AUTO)
	{
		sheight.unit = SUnitAuto;
	}
	else if (htype == CSS_HEIGHT_SET)
	{
		if (hunit == CSS_UNIT_PCT)
		{
			sheight.unit = SUnitPercent;
		}
		else
		{
			sheight.unit = SUnitPoint;
		}
	}
	else
	{
		sheight.unit = SUnitUndefined;
	}
}
void Style_Layout::Render_alignContent(css_computed_style* style)
{
	css_align_content_e align_content_type;
	align_content_type = static_cast<css_align_content_e>(css_computed_align_content(style));
	switch (align_content_type)
	{
	case CSS_ALIGN_CONTENT_FLEX_START:
		salignContent = SAlignFlexStart;
		break;
	case CSS_ALIGN_CONTENT_CENTER:
		salignContent = SAlignCenter;
		break;
	case CSS_ALIGN_CONTENT_FLEX_END:
		salignContent = SAlignFlexEnd;
		break;
	case CSS_ALIGN_CONTENT_STRETCH:
		salignContent = SAlignStretch;
		break;
	case CSS_ALIGN_CONTENT_SPACE_BETWEEN:
		salignContent = SAlignSpaceBetween;
		break;
	case CSS_ALIGN_CONTENT_SPACE_AROUND:
		salignContent = SAlignSpaceAround;
		break;
	case CSS_ALIGN_CONTENT_SPACE_EVENLY:
		salignContent = SAlignAuto;
		break;
	case CSS_ALIGN_CONTENT_INHERIT:
		salignContent = SAlignAuto;
		break;
	}
}
void Style_Layout::Render_alignItems(css_computed_style* style)
{
	css_align_items_e align_items_type;
	align_items_type = static_cast<css_align_items_e>(css_computed_align_items(style));
	switch (align_items_type)
	{
	case CSS_ALIGN_ITEMS_INHERIT:
		salignItems = SAlignAuto;
		break;
	case CSS_ALIGN_ITEMS_STRETCH:
		salignItems = SAlignStretch;
		break;
	case CSS_ALIGN_ITEMS_FLEX_START:
		salignItems = SAlignFlexStart;
		break;
	case CSS_ALIGN_ITEMS_FLEX_END:
		salignItems = SAlignFlexEnd;
		break;
	case CSS_ALIGN_ITEMS_CENTER:
		salignItems = SAlignCenter;
		break;
	case CSS_ALIGN_ITEMS_BASELINE:
		salignItems = SAlignBaseline;
		break;
	}
}
void Style_Layout::Render_alignSelf(css_computed_style* style)
{
	css_align_self_e align_self_type;
	align_self_type = static_cast<css_align_self_e>(css_computed_align_self(style));
	switch (align_self_type)
	{
	case CSS_ALIGN_SELF_INHERIT:
		salignSelf = SAlignAuto;
		break;
	case CSS_ALIGN_SELF_STRETCH:
		salignSelf = SAlignStretch;
		break;
	case CSS_ALIGN_SELF_FLEX_START:
		salignSelf = SAlignFlexStart;
		break;
	case CSS_ALIGN_SELF_FLEX_END:
		salignSelf = SAlignFlexEnd;
		break;
	case CSS_ALIGN_SELF_CENTER:
		salignSelf = SAlignCenter;
		break;
	case CSS_ALIGN_SELF_BASELINE:
		salignSelf = SAlignBaseline;
		break;
	case CSS_ALIGN_SELF_AUTO:
		salignSelf = SAlignAuto;
		break;
	}
}
void Style_Layout::Render_display(css_computed_style* style, bool hasparent)
{
	css_display_e display = static_cast<css_display_e>(css_computed_display(style, hasparent));
	if (display == CSS_DISPLAY_NONE)
	{
		sdisplay = SDisplayNone;
	}
	else
	{
		sdisplay = SDisplayFlex;
	}
}
void Style_Layout::Render_flex_wrap(css_computed_style* style)
{
	css_flex_wrap_e flex_wrap_type = static_cast<css_flex_wrap_e>(css_computed_flex_wrap(style));
	switch (flex_wrap_type)
	{
	case CSS_FLEX_WRAP_INHERIT:
		sflex_wrap = SWrapNoWrap;
		break;
	case CSS_FLEX_WRAP_NOWRAP:
		sflex_wrap = SWrapNoWrap;
		break;
	case CSS_FLEX_WRAP_WRAP:
		sflex_wrap = SWrapWrap;
		break;
	case CSS_FLEX_WRAP_WRAP_REVERSE:
		sflex_wrap = SWrapWrapReverse;
		break;
	}
}
void Style_Layout::Render_flex_grow(css_computed_style* style)
{
	css_fixed flex_grow;
	css_flex_grow_e flex_grow_type = static_cast<css_flex_grow_e>(css_computed_flex_grow(style, &flex_grow));
	flex_grow /= 1024;
	sflex_grow = flex_grow;
}
void Style_Layout::Render_flex_shrink(css_computed_style* style)
{
	css_fixed flex_shrink;
	css_flex_shrink_e flex_shrink_type = static_cast<css_flex_shrink_e>(css_computed_flex_shrink(style, &flex_shrink));
	flex_shrink /= 1024;
	sflex_shrink = flex_shrink;
}
void Style_Layout::Render_flex_basis(css_computed_style* style)
{
	css_fixed flex_basis_value = 0;
	css_unit flex_basis_unit = CSS_UNIT_PX;
	css_flex_basis_e flex_basis_type = CSS_FLEX_BASIS_AUTO;
	flex_basis_type = static_cast<css_flex_basis_e>(css_computed_flex_basis(style, &flex_basis_value, &flex_basis_unit));
	flex_basis_value /= 1024;
	sflex_basis.value = flex_basis_value;
	if (flex_basis_type == CSS_FLEX_BASIS_AUTO)
	{
		sflex_basis.unit = SUnitAuto;
	}
	else if (flex_basis_type == CSS_FLEX_BASIS_SET)
	{
		if (flex_basis_unit == CSS_UNIT_PCT)
		{
			sflex_basis.unit = SUnitPercent;
		}
		else
		{
			sflex_basis.unit = SUnitPoint;
		}
	}
	else
	{
		sflex_basis.unit = SUnitUndefined;
	}
}
void Style_Layout::Render_flex_direction(css_computed_style* style)
{
	css_flex_direction_e flex_wrap_type = static_cast<css_flex_direction_e>(css_computed_flex_direction(style));
	switch (flex_wrap_type)
	{
	case CSS_FLEX_DIRECTION_INHERIT:
		sflex_direction=SFlexDirectionColumn;
		break;
	case CSS_FLEX_DIRECTION_ROW:
		sflex_direction=SFlexDirectionRow;
		break;
	case CSS_FLEX_DIRECTION_ROW_REVERSE:
		sflex_direction=SFlexDirectionRowReverse;
		break;
	case CSS_FLEX_DIRECTION_COLUMN:
		sflex_direction=SFlexDirectionColumn;
		break;
	case CSS_FLEX_DIRECTION_COLUMN_REVERSE:
		sflex_direction=SFlexDirectionColumnReverse;
		break;
	}
}
void Style_Layout::Render_justify(css_computed_style* style)
{
	css_justify_content_e justify_type = static_cast<css_justify_content_e>(css_computed_justify_content(style));
	switch (justify_type)
	{
	case CSS_JUSTIFY_CONTENT_INHERIT:
		break;
	case CSS_JUSTIFY_CONTENT_FLEX_START:
		sjustify = SJustifyFlexStart;
		break;
	case CSS_JUSTIFY_CONTENT_FLEX_END:
		sjustify = SJustifyFlexEnd;
		break;
	case CSS_JUSTIFY_CONTENT_CENTER:
		sjustify = SJustifyCenter;
		break;
	case CSS_JUSTIFY_CONTENT_SPACE_BETWEEN:
		sjustify = SJustifySpaceBetween;
		break;
	case CSS_JUSTIFY_CONTENT_SPACE_AROUND:
		sjustify = SJustifySpaceAround;
		break;
	case CSS_JUSTIFY_CONTENT_SPACE_EVENLY:
		sjustify = SJustifySpaceEvenly;
		break;
	}
}
void Style_Layout::Render_positions(css_computed_style* style)
{
	css_position_e position_type = static_cast<css_position_e>(css_computed_position(style));
	if (position_type == CSS_POSITION_ABSOLUTE)
	{
		spositions.type = SPositionTypeAbsolute;
	}
	else
	{
		spositions.type = SPositionTypeRelative;
	}

	css_fixed value = 0;
	css_unit unit = CSS_UNIT_PX;
	int type = CSS_LEFT_AUTO;
	setValue(style, position_left, value, unit, type);
	spositions.position[SEdgeLeft].value = value;
	if (unit == CSS_UNIT_PCT)
	{
		spositions.position[SEdgeLeft].unit = SUnitPercent;
	}
	else
	{
		spositions.position[SEdgeLeft].unit = SUnitPoint;
	}
	setValue(style, position_top, value, unit, type);
	spositions.position[SEdgeTop].value = value;
	if (unit == CSS_UNIT_PCT)
	{
		spositions.position[SEdgeTop].unit = SUnitPercent;
	}
	else
	{
		spositions.position[SEdgeTop].unit = SUnitPoint;
	}
	setValue(style, position_right, value, unit, type);
	spositions.position[SEdgeRight].value = value;
	if (unit == CSS_UNIT_PCT)
	{
		spositions.position[SEdgeRight].unit = SUnitPercent;
	}
	else
	{
		spositions.position[SEdgeRight].unit = SUnitPoint;
	}
	setValue(style, position_bottom, value, unit, type);
	spositions.position[SEdgeBottom].value = value;
	if (unit == CSS_UNIT_PCT)
	{
		spositions.position[SEdgeBottom].unit = SUnitPercent;
	}
	else
	{
		spositions.position[SEdgeBottom].unit = SUnitPoint;
	}
}
void Style_Layout::Render_margins(css_computed_style* style)
{
	css_fixed value = 0;
	css_unit unit = CSS_UNIT_PX;
	int type = CSS_LEFT_AUTO;
	setValue(style, margin_left, value, unit, type);
	smargins.margin[SEdgeLeft].value = value;
	if (type == CSS_WIDTH_AUTO)
	{
		smargins.margin[SEdgeLeft].unit = SUnitAuto;
	}
	else if (type == CSS_WIDTH_SET)
	{
		if (unit == CSS_UNIT_PCT)
		{
			smargins.margin[SEdgeLeft].unit = SUnitPercent;
		}
		else
		{
			smargins.margin[SEdgeLeft].unit = SUnitPoint;
		}
	}
	else
	{
		smargins.margin[SEdgeLeft].unit = SUnitUndefined;
	}

	setValue(style, margin_top, value, unit, type);
	smargins.margin[SEdgeTop].value = value;
	if (type == CSS_WIDTH_AUTO)
	{
		smargins.margin[SEdgeTop].unit = SUnitAuto;
	}
	else if (type == CSS_WIDTH_SET)
	{
		if (unit == CSS_UNIT_PCT)
		{
			smargins.margin[SEdgeTop].unit = SUnitPercent;
		}
		else
		{
			smargins.margin[SEdgeTop].unit = SUnitPoint;
		}
	}
	else
	{
		smargins.margin[SEdgeTop].unit = SUnitUndefined;
	}

	setValue(style, margin_right, value, unit, type);
	smargins.margin[SEdgeRight].value = value;
	if (type == CSS_WIDTH_AUTO)
	{
		smargins.margin[SEdgeRight].unit = SUnitAuto;
	}
	else if (type == CSS_WIDTH_SET)
	{
		if (unit == CSS_UNIT_PCT)
		{
			smargins.margin[SEdgeRight].unit = SUnitPercent;
		}
		else
		{
			smargins.margin[SEdgeRight].unit = SUnitPoint;
		}
	}
	else
	{
		smargins.margin[SEdgeRight].unit = SUnitUndefined;
	}

	setValue(style, margin_bottom, value, unit, type);
	smargins.margin[SEdgeBottom].value = value;
	if (type == CSS_WIDTH_AUTO)
	{
		smargins.margin[SEdgeBottom].unit = SUnitAuto;
	}
	else if (type == CSS_WIDTH_SET)
	{
		if (unit == CSS_UNIT_PCT)
		{
			smargins.margin[SEdgeBottom].unit = SUnitPercent;
		}
		else
		{
			smargins.margin[SEdgeBottom].unit = SUnitPoint;
		}
	}
	else
	{
		smargins.margin[SEdgeBottom].unit = SUnitUndefined;
	}
}
void Style_Layout::Render_borders(css_computed_style* style)
{
	css_fixed value = 0;
	css_unit unit = CSS_UNIT_PX;
	int type = CSS_BORDER_WIDTH_MEDIUM;
	css_border_width_e t;
	setValue(style, border_left, value, unit, type);
	t = static_cast<css_border_width_e>(type);
	sborders.type[SEdgeLeft] = static_cast<SBorderType>(css_computed_border_left_style(style));
	switch (t)
	{
	case CSS_BORDER_WIDTH_INHERIT:
		break;
	case CSS_BORDER_WIDTH_THIN:
		sborders.border[SEdgeLeft] = 1;
		break;
	case CSS_BORDER_WIDTH_MEDIUM:
		sborders.border[SEdgeLeft] = 3;
		break;
	case CSS_BORDER_WIDTH_THICK:
		sborders.border[SEdgeLeft] = 5;
		break;
	case CSS_BORDER_WIDTH_WIDTH:
		sborders.border[SEdgeLeft] = value;
		break;
	}

	setValue(style, border_top, value, unit, type);
	t = static_cast<css_border_width_e>(type);
	sborders.type[SEdgeTop] = static_cast<SBorderType>(css_computed_border_top_style(style));
	switch (t)
	{
	case CSS_BORDER_WIDTH_INHERIT:
		break;
	case CSS_BORDER_WIDTH_THIN:
		sborders.border[SEdgeTop] = 1;
		break;
	case CSS_BORDER_WIDTH_MEDIUM:
		sborders.border[SEdgeTop] = 3;
		break;
	case CSS_BORDER_WIDTH_THICK:
		sborders.border[SEdgeTop] = 5;
		break;
	case CSS_BORDER_WIDTH_WIDTH:
		sborders.border[SEdgeTop] = value;
		break;
	}

	setValue(style, border_right, value, unit, type);
	t = static_cast<css_border_width_e>(type);
	sborders.type[SEdgeRight] = static_cast<SBorderType>(css_computed_border_right_style(style));
	switch (t)
	{
	case CSS_BORDER_WIDTH_INHERIT:
		break;
	case CSS_BORDER_WIDTH_THIN:
		sborders.border[SEdgeRight] = 1;
		break;
	case CSS_BORDER_WIDTH_MEDIUM:
		sborders.border[SEdgeRight] = 3;
		break;
	case CSS_BORDER_WIDTH_THICK:
		sborders.border[SEdgeRight] = 5;
		break;
	case CSS_BORDER_WIDTH_WIDTH:
		sborders.border[SEdgeRight] = value;
		break;
	}

	setValue(style, border_bottom, value, unit, type);
	t = static_cast<css_border_width_e>(type);
	sborders.type[SEdgeBottom] = static_cast<SBorderType>(css_computed_border_bottom_style(style));
	switch (t)
	{
	case CSS_BORDER_WIDTH_INHERIT:
		break;
	case CSS_BORDER_WIDTH_THIN:
		sborders.border[SEdgeBottom] = 1;
		break;
	case CSS_BORDER_WIDTH_MEDIUM:
		sborders.border[SEdgeBottom] = 3;
		break;
	case CSS_BORDER_WIDTH_THICK:
		sborders.border[SEdgeBottom] = 5;
		break;
	case CSS_BORDER_WIDTH_WIDTH:
		sborders.border[SEdgeBottom] = value;
		break;
	}
}
void Style_Layout::Render_paddings(css_computed_style* style)
{
	css_fixed value = 0;
	css_unit unit = CSS_UNIT_PX;
	int type = CSS_LEFT_AUTO;
	setValue(style, padding_left, value, unit, type);
	if (type == CSS_MIN_WIDTH_SET)
	{
		spaddings.padding[SEdgeLeft].value = value;
		if (type == CSS_WIDTH_SET)
		{
			if (unit == CSS_UNIT_PCT)
			{
				spaddings.padding[SEdgeLeft].unit = SUnitPercent;
			}
			else
			{
				spaddings.padding[SEdgeLeft].unit = SUnitPoint;
			}
		}
	}

	setValue(style, padding_top, value, unit, type);
	if (type == CSS_MIN_WIDTH_SET)
	{
		spaddings.padding[SEdgeTop].value = value;
		if (type == CSS_WIDTH_SET)
		{
			if (unit == CSS_UNIT_PCT)
			{
				spaddings.padding[SEdgeTop].unit = SUnitPercent;
			}
			else
			{
				spaddings.padding[SEdgeTop].unit = SUnitPoint;
			}
		}
	}

	setValue(style, padding_right, value, unit, type);
	if (type == CSS_MIN_WIDTH_SET)
	{
		spaddings.padding[SEdgeRight].value = value;
		if (type == CSS_WIDTH_SET)
		{
			if (unit == CSS_UNIT_PCT)
			{
				spaddings.padding[SEdgeRight].unit = SUnitPercent;
			}
			else
			{
				spaddings.padding[SEdgeRight].unit = SUnitPoint;
			}
		}
	}

	setValue(style, padding_bottom, value, unit, type);
	if (type == CSS_MIN_WIDTH_SET)
	{
		spaddings.padding[SEdgeBottom].value = value;
		if (type == CSS_WIDTH_SET)
		{
			if (unit == CSS_UNIT_PCT)
			{
				spaddings.padding[SEdgeBottom].unit = SUnitPercent;
			}
			else
			{
				spaddings.padding[SEdgeBottom].unit = SUnitPoint;
			}
		}
	}
}
void Style_Layout::Render_min_width(css_computed_style* style)
{
	css_fixed value = 0;
	css_unit unit = CSS_UNIT_PX;
	int type = CSS_LEFT_AUTO;
	setValue(style, min_width, value, unit, type);
	if (type == CSS_MIN_WIDTH_SET)
	{
		smin_width.value = value;
		if (type == CSS_WIDTH_SET)
		{
			if (unit == CSS_UNIT_PCT)
			{
				smin_width.unit = SUnitPercent;
			}
			else
			{
				smin_width.unit = SUnitPoint;
			}
		}
	}
}
void Style_Layout::Render_min_height(css_computed_style* style)
{
	css_fixed value = 0;
	css_unit unit = CSS_UNIT_PX;
	int type = CSS_LEFT_AUTO;
	setValue(style, min_height, value, unit, type);
	if (type == CSS_MIN_WIDTH_SET)
	{
		smin_height.value = value;
		if (type == CSS_WIDTH_SET)
		{
			if (unit == CSS_UNIT_PCT)
			{
				smin_height.unit = SUnitPercent;
			}
			else
			{
				smin_height.unit = SUnitPoint;
			}
		}
	}
}
void Style_Layout::Render_max_width(css_computed_style* style)
{
	css_fixed value = 0;
	css_unit unit = CSS_UNIT_PX;
	int type = CSS_LEFT_AUTO;
	setValue(style, max_width, value, unit, type);
	if (type == CSS_MIN_WIDTH_SET)
	{
		smax_width.value = value;
		if (type == CSS_WIDTH_SET)
		{
			if (unit == CSS_UNIT_PCT)
			{
				smax_width.unit = SUnitPercent;
			}
			else
			{
				smax_width.unit = SUnitPoint;
			}
		}
	}
}
void Style_Layout::Render_max_height(css_computed_style* style)
{
	css_fixed value = 0;
	css_unit unit = CSS_UNIT_PX;
	int type = CSS_LEFT_AUTO;
	setValue(style, max_height, value, unit, type);
	if (type == CSS_MIN_WIDTH_SET)
	{
		smax_height.value = value;
		if (type == CSS_WIDTH_SET)
		{
			if (unit == CSS_UNIT_PCT)
			{
				smax_height.unit = SUnitPercent;
			}
			else
			{
				smax_height.unit = SUnitPoint;
			}
		}
	}
}
void Style_Layout::print()
{
	printf("\n--------------\n");
	printf("\nwidth: ");
	swidth.print();
	printf("\nheight: ");
	sheight.print();
	printf("\nalignContent: ");
	switch (salignContent)
	{
	case SAlignAuto:
		printf("auto");
		break;
	case SAlignFlexStart:
		printf("flex-start");
		break;
	case SAlignCenter:
		printf("center");
		break;
	case SAlignFlexEnd:
		printf("flex-end");
		break;
	case SAlignStretch:
		printf("stretch");
		break;
	case SAlignBaseline:
		printf("baseline");
		break;
	case SAlignSpaceBetween:
		printf("between");
		break;
	case SAlignSpaceAround:
		printf("around");
		break;
	}
	printf("\nalignItems: ");
	switch (salignItems)
	{
	case SAlignAuto:
		printf("auto");
		break;
	case SAlignFlexStart:
		printf("flex-start");
		break;
	case SAlignCenter:
		printf("center");
		break;
	case SAlignFlexEnd:
		printf("flex-end");
		break;
	case SAlignStretch:
		printf("stretch");
		break;
	case SAlignBaseline:
		printf("baseline");
		break;
	case SAlignSpaceBetween:
		printf("between");
		break;
	case SAlignSpaceAround:
		printf("around");
		break;
	}
	printf("\nalignSelf: ");
	switch (salignSelf)
	{
	case SAlignAuto:
		printf("auto");
		break;
	case SAlignFlexStart:
		printf("flex-start");
		break;
	case SAlignCenter:
		printf("center");
		break;
	case SAlignFlexEnd:
		printf("flex-end");
		break;
	case SAlignStretch:
		printf("stretch");
		break;
	case SAlignBaseline:
		printf("baseline");
		break;
	case SAlignSpaceBetween:
		printf("between");
		break;
	case SAlignSpaceAround:
		printf("around");
		break;
	}
	printf("\ndisplay: ");
	switch (sdisplay)
	{
	case SDisplayFlex:
		printf("flex");
		break;
	case SDisplayNone:
		printf("none");
		break;
	}
	printf("\nflex_wrap: ");
	switch (sflex_wrap)
	{
	case SWrapNoWrap:
		printf("no-wrap");
		break;
	case SWrapWrap:
		printf("wrap");
		break;
	case SWrapWrapReverse:
		printf("wrap-reverse");
		break;
	}
	printf("\nflex_grow:%f",sflex_grow);
	printf("\nflex_shrink:%f",sflex_shrink);
	printf("\nflex_basis: ");
	sflex_basis.print();
	printf("\nflex_direction: ");
	switch (sflex_direction)
	{
	case SFlexDirectionColumn:
		printf("column");
		break;
	case SFlexDirectionColumnReverse:
		printf("column-reverse");
		break;
	case SFlexDirectionRow:
		printf("row");
		break;
	case SFlexDirectionRowReverse:
		printf("row-reverse");
		break;
	}
	printf("\njustify: ");
	switch (sjustify)
	{
	case SJustifyFlexStart:
		printf("flex-start");
		break;
	case SJustifyCenter:
		printf("center");
		break;
	case SJustifyFlexEnd:
		printf("flex-end");
		break;
	case SJustifySpaceBetween:
		printf("space-between");
		break;
	case SJustifySpaceAround:
		printf("around");
		break;
	case SJustifySpaceEvenly:
		printf("space-evenly");
		break;
	}
	printf("\nposition: ");
	spositions.print();
	printf("\nmargin: ");
	smargins.print();
	printf("\nborder: ");
	sborders.print();
	printf("\npadding: ");
	spaddings.print();
	printf("\n--------------\n");
}
