//
// Created by liulizhang on 2020/7/10.
//

#ifndef _RENDER_H_
#define _RENDER_H_
#include "RenderNode.h"
class Render
{
	YGConfig* ygConfig;
 public:
	YGSize (* measure)(YGNodeRef node, float width, YGMeasureMode widthMode, float height, YGMeasureMode heightMode);
	RenderNode* build(DomNode* root, RenderNode* fa);
	RenderNode* root;
	void Render_Tree();
	void Layout_Tree();
	void Print_Tree();
	void Clean_Tree();
};

#endif //_RENDER_H_
