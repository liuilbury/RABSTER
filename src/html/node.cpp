//
// Created by liulizhang on 2020/6/17.
//

#include "node.h"
#include <queue>
#include "../lex/CSSLex.h"
std::set<node*> node::update()
{
	std::set<node*>changeNode;
	std::queue<node*>q;
	q.push(this);
	int flag=1;
	while(!q.empty()){
		node* now=q.front();
		q.pop();
		for(auto i:this->_changeSet){
			if(now->vis[i]==0){
				flag=1;
				//float *color=this->_style.color;
				std::string vv;
				//now->_final_style.color[0]=color[0];
				//now->_final_style.color[1]=color[1];
				//now->_final_style.color[2]=color[2];
			}
		}
		if(flag)
		{
			changeNode.insert(now);
			for (auto i:now->_children)
			{
				q.push(i);
			}
		}
		flag=0;
	}
	return changeNode;
}
void node::add_child(node* c)
{
	this->_children.push_back(c);
	c->_parent=this;
}
void node::change(int key, const std::string &value)
{
	vis[key]=true;
	_changeSet.insert(key);
	//_style.set(key,value);
	//_final_style.set(key,value);
}
void node::Render_Box(int nWidth,int nHeight,css_pseudo_element mod)
{
	css_fixed width = 0;
	css_unit wunit = CSS_UNIT_PX;
	css_width_e wtype = CSS_WIDTH_AUTO;
	css_fixed height = 0;
	css_height_e htype = CSS_HEIGHT_AUTO;
	css_unit hunit = CSS_UNIT_PX;
	css_box_sizing_e bs = CSS_BOX_SIZING_CONTENT_BOX;
	wtype = static_cast<css_width_e>(css_computed_width(_final_style->styles[mod], &width, &wunit));
	htype = static_cast<css_height_e>(css_computed_height(_final_style->styles[mod], &height, &hunit));

	width=width/1024;
	height=height/1024;
	bs = static_cast<css_box_sizing_e>(css_computed_box_sizing(_final_style->styles[mod]));
	if (wtype == CSS_WIDTH_AUTO)
	{
		if (_parent == nullptr)
		{
			width = nWidth;
		}
		else
		{
			width =_parent->box.width;
		}
	}
	else if (wunit == CSS_UNIT_PCT)
	{
		width = _parent->box.width * width / 100;
	}
	if (_parent != nullptr)
	{
		width = min(width, _parent->box.width);
	}
	if (htype == CSS_HEIGHT_AUTO)
	{
		if (_parent == nullptr)
		{
			height = nHeight;
		}
		else
		{
			height = _parent->box.height;
		}
	}
	else if (hunit == CSS_UNIT_PCT)
	{
		height = _parent->box.height * height / 100;
	}
	if (_parent != nullptr)
	{
		height = min(height, _parent->box.height);
	}
	box.width=width;
	box.height=height;
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
void node::Render_display()
{
	box.display= static_cast<css_display_e>(css_computed_display(_final_style->styles[CSS_PSEUDO_ELEMENT_NONE], _parent != nullptr));
}
void node::Render_Time()
{
	css_fixed delay_time=0;
	css_transition_delay_e delayE;
	css_unit delay_unit = CSS_UNIT_S;
	delayE= static_cast<css_transition_delay_e>(css_computed_transition_delay(_final_style->styles[CSS_PSEUDO_ELEMENT_NONE], &delay_time, &delay_unit));
	css_fixed duration_time=0;
	css_transition_duration_e durationE;
	css_unit duration_unit = CSS_UNIT_S;
	durationE= static_cast<css_transition_duration_e>(css_computed_transition_duration(_final_style->styles[CSS_PSEUDO_ELEMENT_NONE], &duration_time, &duration_unit));
	delay_time=delay_time*1000/1024;
	duration_time=duration_time*1000/1024;
	box.start_time=delay_time;
	box.end_time=box.start_time+duration_time;
}
