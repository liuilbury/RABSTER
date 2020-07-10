//
// Created by liulizhang on 2020/6/17.
//

#ifndef _NODE_H_
#define _NODE_H_

#include <set>
#include <list>
#include <libcss/libcss.h>
#include "gumbo.h"
#include "Yoga.h"
#include "YGStyle.h"
#include "YGLayout.h"
#include "YGNode.h"
struct Box{
	int x=0,y=0;
	int width=0;
	int height=0;
	int color[4];
	int position[4];
	css_position_e position_type;
	css_border_style_e border_style[4];
	int border_color[4][4];
	css_display_e display;
	int start_time=0;
	int end_time=0;
	css_direction_e direction;
	css_align_content_e align_content;
	YGValue flex_grow;
	css_flex_direction_e flex_direction;
	YGValue flex_basis;
	YGValue flex_shrink;
	css_flex_wrap_e flex_wrap;
	void to_string(){
		printf("width=%d height=%d\n",width,height);
		printf("position:%d %d %d %d\n",position[0],position[1],position[2],position[3]);
		printf("color=%d %d %d %d\n",color[0],color[1],color[2],color[3]);
		printf("time:%d %d\n",start_time,end_time);
	}
};
class node
{
 public:
	node()
	{
		node_data = nullptr;
		name = nullptr;
		real_name = "";
		n_classes = 0;
		classes= nullptr;
		Id= nullptr;
		_changeSet.clear();
		_parent=_next=_prev=_last_child= nullptr;
		_children.clear();
		_inline_style= "";
		_final_style= nullptr;
		hover= false;
		link= nullptr;
	}
	YGNodeRef ygnode;
	GumboNode* htmlnode;
	void* node_data;
	bool vis[145] = { false };
	lwc_string* name;
	std::string real_name;
	uint32_t n_classes;
	lwc_string** classes;
	lwc_string* Id;
	std::set<int> _changeSet;
	node* _parent;
	node* _next;
	node* _prev;
	node* _last_child;
	std::list<node*> _children;
	std::string _inline_style;
	css_select_results* _style;
	css_select_results* _final_style;
	Box save_style;
	std::list<std::string>url;
	void change(int key, const std::string& value);
	std::set<node*> update();
	void add_child(node*);
	void Render_Box(css_pseudo_element);
	void Render_Init_Box(css_pseudo_element);
	void Render_Color(css_pseudo_element mod);
	void Render_Display(css_computed_style* style);
	void Render_Time();
	Box box;
	bool hover;
	node *link;
	void Render_Width(css_computed_style* style);
	void Render_Height(css_computed_style* style);
	void Render_Flex_Basis(css_computed_style* style);
	void Render_Box_Sizing(css_computed_style* style);
	void Render_Flex_Direction(css_computed_style* style);
	void Render_Flex_Grow(css_computed_style* style);
	void Render_Flex_Shrink(css_computed_style* style);
	void Render_Position(css_computed_style* style);
	void Render_Flex_Wrap(css_computed_style* style);
	void Render_Direction(css_computed_style* style);
	void Render_Align_Content(css_computed_style* style);
	void Render_Position_Type(css_computed_style* style);
	void Render_Flex(css_computed_style* style);
	void Render_Border_Width(css_computed_style* style);
	void Render_Border_Style(css_computed_style* style);
	void Render_Border_Color(css_computed_style* style);
	void Render_Border_Padding(css_computed_style* style);
};

#endif //_NODE_H_
