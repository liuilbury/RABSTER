//
// Created by liulizhang on 2020/7/9.
//

#ifndef _DOMNODE_H_
#define _DOMNODE_H_
#include <list>
#include <string>
#include <libcss/libcss.h>
class DomNode
{
 public:
	DomNode()
	{
		node_data = nullptr;
		name = nullptr;
		real_name = "";
		n_classes = 0;
		classes = nullptr;
		Id = nullptr;
		_parent = _next = _prev = _last_child = nullptr;
		_children.clear();
		_inline_style = "";
		_final_style = nullptr;
		hover = false;
		link = nullptr;
	}
	lwc_string* name;
	std::string real_name;
	bool hover;
	DomNode* _parent;
	DomNode* _next;
	DomNode* _prev;
	DomNode* _last_child;
	std::list<DomNode*> _children;
	std::string _inline_style;
	DomNode* link;
	lwc_string* Id;
	uint32_t n_classes;
	lwc_string** classes;
	void* node_data;
	css_select_results* _style;
	css_select_results* _final_style;
};

#endif //_DOMNODE_H_
