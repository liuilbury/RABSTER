//
// Created by liulizhang on 2020/7/10.
//

#ifndef _NODE_H_
#define _NODE_H_
template<class _node>
class Node
{
	std::string real_name;
	_node* _parent;
	_node* _next;
	_node* _prev;
	_node* _last_child;
	std::list<_node*> _children;

 public:
	void setName(std::string name){
		real_name=name;
	}
	std::string get_Name()
	{
		return real_name;
	}
	void setParent(_node* pa)
	{
		_parent = pa;
	}
	_node* getParent()
	{
		return _parent;
	}
	void setchild(_node* child){
		_children.push_back(child);
	}
	std::list<_node*> getChildren()
	{
		return _children;
	}
	Node()
	{
		real_name.clear();
		_parent = nullptr;
		_next = nullptr;
		_prev = nullptr;
		_last_child = nullptr;
		_children.clear();
	}
};
#endif //_NODE_H_
