//
// Created by liulizhang on 2020/7/10.
//

#include <cstring>
#include "Render.h"
void Render::Render_Tree()
{
	size_t group=0;
	ygConfig = YGConfigNew();
	std::queue<RenderNode*> q;
	q.push(root);
	while (!q.empty())
	{
		RenderNode* d = q.front();
		q.pop();
		d->ygNode = YGNodeNewWithConfig(ygConfig);
		d->ygNode->setContext(d);
		d->Render_Node();
		if(d->element->type==GUMBO_NODE_TEXT)
		{
			if(d->text==" "){
				group++;
				d->ygNode->SetGruop(group);
				d->ygNode->setMeasureFunc(measure);
				group++;
				continue;
			}
			d->ygNode->SetGruop(group);
			d->ygNode->setMeasureFunc(measure);
			continue;
		}else{
			group++;
			d->ygNode->SetGruop(group);
		}
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
		int cnt = 0;
		for (auto i:d->getChildren())
		{
			YGNodeInsertChild(d->ygNode, i->ygNode, cnt++);
			q.push(i);
		}
	}
}
void Render::Print_Tree()
{
	std::queue<RenderNode*> q;
	q.push(root);
	while (!q.empty())
	{
		RenderNode* d = q.front();
		q.pop();
		d->print();
		for (auto i:d->getChildren())
		{
			q.push(i);
		}
	}
}
RenderNode* Render::build(DomNode* dom, RenderNode* fa)
{
	RenderNode* renderNode = new RenderNode();
	renderNode->text = dom->text;
	renderNode->_final_style = dom->_final_style;
	renderNode->setName(dom->real_name);
	renderNode->setParent(fa);
	renderNode->element = dom->element;
	for (auto i:dom->_children)
	{
		renderNode->setchild(build(i, renderNode));
	}
	RenderNode* last = nullptr;
	for (auto i:renderNode->getChildren())
	{
		if (last != nullptr)
		{
			i->setBrother(last);
		}
		last = i;
	}
	return renderNode;
}
void Render::Clean_Tree()
{
	std::queue<RenderNode*> q;
	q.push(root);
	while (!q.empty())
	{
		RenderNode* d = q.front();
		q.pop();
		d->style->StyleLayout.vis=false;
		for (auto i:d->getChildren())
		{
			q.push(i);
		}
	}
}

