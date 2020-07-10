//
// Created by liulizhang on 2020/7/9.
//

#include "RenderNode.h"
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
		YGNodeStyleSetWidth(ygNode, layout->swidth.value);
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
		YGNodeStyleSetHeight(ygNode, layout->sheight.value);
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
	//justify
	YGNodeStyleSetJustifyContent(ygNode, (YGJustify)layout->sjustify);
	//position_type
	YGNodeStyleSetPositionType(ygNode, (YGPositionType)layout->spositions.type);
	////////////////
	for (int i = 0; i < 3; i++)
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
		YGNodeStyleSetBorder(ygNode, ygEdge, layout->sborders.border[ygEdge].value);
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
