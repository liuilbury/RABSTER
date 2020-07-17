//
// Created by liulizhang on 2020/7/10.
//

#include <cstring>
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
		d->ygNode = YGNodeNewWithConfig(ygConfig);
		d->ygNode->setContext(d);
		d->Render_Node();
		if(d->element->type==GUMBO_NODE_TEXT)
		{
			d->ygNode->setMeasureFunc(Render::measure);
			continue;
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
		if(d->element->type==GUMBO_NODE_TEXT)
		{
			continue;
		}
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
	char* data = std::strtok(dom->text.data(), " ");
	while (data != nullptr)
	{
		renderNode->data.push_back(data);
		data = std::strtok(NULL, " ");
	}
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
		if(d->element== nullptr)d->style->StyleLayout.vis=true;
		for (auto i:d->getChildren())
		{
			q.push(i);
		}
	}
}

