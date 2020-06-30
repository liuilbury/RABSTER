//
// Created by liulizhang on 2020/6/17.
//

#ifndef _NODE_H_
#define _NODE_H_

#include "style.h"
#include <set>
#include <list>
#include <libcss/libcss.h>
using namespace std;
struct Box{
	int x,y;
	int width;
	int height;
	int color[4];
	css_display_e display;
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
	}
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
	void change(int key, const std::string& value);
	set<node*> update();
	void add_child(node*);
	void Render_Box(int nWidth,int nHeight);
	void Render_Color();
	void Render_display();
	Box box;
};

#endif //_NODE_H_
