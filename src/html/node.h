//
// Created by liulizhang on 2020/6/17.
//

#ifndef _NODE_H_
#define _NODE_H_

#include "style.h"
#include <set>
#include <list>
#include <libcss/libcss.h>
#include "gumbo.h"
#include "Yoga.h"
#include "YGStyle.h"
#include "YGLayout.h"
using namespace std;
struct Box{
	int x=0,y=0;
	int width=0;
	int height=0;
	int color[4];
	css_display_e display;
	int start_time=0;
	int end_time=0;
	void to_string(){
		printf("width=%d height=%d\n",width,height);
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
	list<node*> _children;
	std::string _inline_style;
	css_select_results* _style;
	css_select_results* _final_style;
	Box save_style;
	std::list<std::string>url;
	void change(int key, const std::string& value);
	set<node*> update();
	void add_child(node*);
	void Render_Box(int nWidth,int nHeight,css_pseudo_element);
	void Render_Color(css_pseudo_element mod);
	void Render_Display(css_computed_style* style);
	void Render_Time();
	Box box;
	bool hover;
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
};

#endif //_NODE_H_
