//
// Created by liulizhang on 2020/7/10.
//

#include "Render.h"
void Render::Render_Tree()
{
	ygConfig = YGConfigNew();
	std::queue<RenderNode*> q;
	q.push(root);
	while (!q.empty())
	{
		RenderNode* d = q.front();
		q.pop();
		d->ygNode=YGNodeNewWithConfig(ygConfig);
		d->Render_Node();
		for (auto i:d->getChildren())
		{
			q.push(i);
		}
	}
}
void Render::Layout_Tree()
{
	std::queue<RenderNode*> q;
	q.push(root);
	while (!q.empty())
	{
		RenderNode* d = q.front();
		q.pop();
		d->Calculate_Layout();
		int cnt=0;
		for (auto i:d->getChildren())
		{
			YGNodeInsertChild(d->ygNode,i->ygNode,cnt++);
			q.push(i);
		}
	}
}
void Render::Print_Tree()
{
}
RenderNode* Render::build(DomNode* root,RenderNode* fa)
{
	RenderNode* renderNode=new RenderNode();
	renderNode->_final_style=root->_final_style;
	renderNode->setName(root->real_name);
	renderNode->setParent(fa);
	for(auto i:root->_children){
		renderNode->setchild(build(i,renderNode));
	}
	return renderNode;
}
