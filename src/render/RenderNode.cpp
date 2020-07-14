//
// Created by liulizhang on 2020/7/9.
//

#include "RenderNode.h"
#include <gumbo.h>
void RenderNode::Render_Node()
{
	style->Render_Style(_final_style->styles[0], getParent() != nullptr);
}
void RenderNode::Calculate_Layout()
{
	Style_Layout* layout = style->get_Layout();
	//width
	if ((YGUnit)layout->swidth.unit == YGUnitAuto)
	{
		YGNodeStyleSetWidthAuto(ygNode);
	}
	else if ((YGUnit)layout->swidth.unit == YGUnitPercent)
	{
		YGNodeStyleSetWidthPercent(ygNode, layout->swidth.value);
	}
	else
	{
		///
		float width = layout->swidth.value;
		if (layout->sborders.type[YGEdgeLeft] != SBorderTypeNone)
		{
			width += layout->sborders.border[YGEdgeLeft];
		}
		if (layout->sborders.type[YGEdgeRight] != SBorderTypeNone)
		{
			width += layout->sborders.border[YGEdgeRight];
		}
		YGNodeStyleSetWidth(ygNode, width);
	}
	//height
	if ((YGUnit)layout->sheight.unit == YGUnitAuto)
	{
		YGNodeStyleSetHeightAuto(ygNode);
	}
	else if ((YGUnit)layout->sheight.unit == YGUnitPercent)
	{
		YGNodeStyleSetHeightPercent(ygNode, layout->sheight.value);
	}
	else
	{
		float height = layout->sheight.value;
		if (layout->sborders.type[YGEdgeTop] != SBorderTypeNone)
		{
			height += layout->sborders.border[YGEdgeTop];
		}
		if (layout->sborders.type[YGEdgeBottom] != SBorderTypeNone)
		{
			height += layout->sborders.border[YGEdgeBottom];
		}
		YGNodeStyleSetHeight(ygNode, height);
	}
	//align_content
	YGNodeStyleSetAlignContent(ygNode, (YGAlign)layout->salignContent);
	//align_items
	YGNodeStyleSetAlignItems(ygNode, (YGAlign)layout->salignItems);
	//align_self
	YGNodeStyleSetAlignSelf(ygNode, (YGAlign)layout->salignSelf);
	//display
	YGNodeStyleSetDisplay(ygNode, (YGDisplay)layout->sdisplay);
	//flex_wrap
	YGNodeStyleSetFlexWrap(ygNode, (YGWrap)layout->sflex_wrap);
	//flex_grow
	YGNodeStyleSetFlexGrow(ygNode, layout->sflex_grow);
	//flex_shrink
	YGNodeStyleSetFlexShrink(ygNode, layout->sflex_shrink);
	//flex_basis
	if ((YGUnit)layout->sflex_basis.unit == YGUnitAuto)
	{
		YGNodeStyleSetFlexBasisAuto(ygNode);
	}
	else if ((YGUnit)layout->sflex_basis.unit == YGUnitPercent)
	{
		YGNodeStyleSetFlexBasisPercent(ygNode, layout->sflex_basis.value);
	}
	else
	{
		YGNodeStyleSetFlexBasis(ygNode, layout->sflex_basis.value);
	}
	//flex_direction;
	YGNodeStyleSetFlexDirection(ygNode, (YGFlexDirection)layout->sflex_direction);
	//justify
	YGNodeStyleSetJustifyContent(ygNode, (YGJustify)layout->sjustify);
	//position_type
	YGNodeStyleSetPositionType(ygNode, (YGPositionType)layout->spositions.type);
	////////////////
	for (int i = 0; i < 4; i++)
	{
		YGEdge ygEdge = (YGEdge)i;
		//position
		YGNodeStyleSetPosition(ygNode, ygEdge, layout->spositions.position[ygEdge].value);
		//margin
		if ((YGUnit)layout->smargins.margin[ygEdge].unit == YGUnitAuto)
		{
			YGNodeStyleSetMarginAuto(ygNode, ygEdge);
		}
		else if ((YGUnit)layout->smargins.margin[ygEdge].unit == YGUnitPercent)
		{
			YGNodeStyleSetMarginPercent(ygNode, ygEdge, layout->smargins.margin[ygEdge].value);
		}
		else
		{
			YGNodeStyleSetMargin(ygNode, ygEdge, layout->smargins.margin[ygEdge].value);
		}
		//border
		if (layout->sborders.type[ygEdge] != SBorderTypeNone)
			YGNodeStyleSetBorder(ygNode, ygEdge, layout->sborders.border[ygEdge]);
		//padding
		if ((YGUnit)layout->spaddings.padding[ygEdge].unit == YGUnitPercent)
		{
			YGNodeStyleSetPaddingPercent(ygNode, ygEdge, layout->spaddings.padding[ygEdge].value);
		}
		else
		{
			YGNodeStyleSetPadding(ygNode, ygEdge, layout->spaddings.padding[ygEdge].value);
		}
	}
}
void RenderNode::print()
{
	printf("%s\n", get_Name().data());
	style->print();
}
YGSize RenderNode::measure()
{
	GumboNode* dom = element;
	return YGSize({ 1, 2 });
}

