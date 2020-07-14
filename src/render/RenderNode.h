//
// Created by liulizhang on 2020/7/9.
//

#ifndef _RENDERNODE_H_
#define _RENDERNODE_H_
#include "html/html.h"
#include "Style.h"
class RenderNode : public Node<RenderNode>
{
 public:
	GumboNode* element;
	Style* style;
	css_select_results* _final_style;
	YGNode *ygNode;
	void Render_Node();
	void Calculate_Layout();
	RenderNode(){
		Node<RenderNode>();
		_final_style=new css_select_results();
		style=new Style();
	}
	void print();
	YGSize measure();
};

#endif //_RENDERNODE_H_
