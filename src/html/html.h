#include "HtmlContent.h"
#include "node.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
node* build_html_tree(node* fa, rapidjson::Value& dom)
{
	node* now = new node();
#ifdef DEBUG_HTML
	assert(dom.IsObject());
	for (auto& m : dom.GetObject())
	{
		printf("%s ", m.name.GetString());
		std::string s=m.name.GetString();
		if(m.value.IsString()){
			printf("is %s|",m.value.GetString());
		}else{
			printf("|");
		}
	}
	printf("\n");
#endif
	if (fa == nullptr)
	{
		std::string s = "root";
		now->real_name = s;
		lwc_intern_string(s.c_str(), strlen(s.c_str()), &now->name);
	}
	now->_parent = fa;
	if (dom.HasMember("name"))
	{
		std::string s = dom["name"].GetString();
		now->real_name = s;
		lwc_intern_string(s.c_str(), strlen(s.c_str()), &now->name);
	}
	if (dom.HasMember("class"))
	{
		if (dom["class"].IsArray())
		{
			now->n_classes = dom["class"].Size();
			now->classes = (lwc_string**)malloc(sizeof(lwc_string*) * now->n_classes);
			for (rapidjson::SizeType i = 0; i < now->n_classes; i++)
			{
				std::string s = dom["class"][i]["name"].GetString();
				lwc_intern_string(s.c_str(), strlen(s.c_str()), &now->classes[i]);
			}
		}
		else if (dom["class"].IsObject())
		{
			now->n_classes = 1;
			now->classes = (lwc_string**)malloc(sizeof(lwc_string*) * now->n_classes);
			std::string s = dom["class"]["name"].GetString();
			lwc_intern_string(s.c_str(), strlen(s.c_str()), &now->classes[0]);
		}
	}
	if (dom.HasMember("id"))
	{
		std::string s = dom["name"].GetString();
		lwc_intern_string(s.c_str(), strlen(s.c_str()), &now->Id);
	}
	if (dom.HasMember("style"))
	{
		now->_inline_style = dom["style"].GetString();
	}
	if (dom.HasMember("child"))
	{
		if (dom["child"].IsArray())
		{
			node* n= nullptr, * _last_child= nullptr;
			for (rapidjson::SizeType i = 0; i < dom["child"].Size(); i++)
			{
				n = build_html_tree(now, dom["child"][i]);
				now->_children.push_back(n);
				n->_prev = _last_child;
				if (_last_child != nullptr)
					_last_child->_next = n;
				_last_child = n;
			}
		}
		else if (dom["child"].IsObject())
		{
			now->_children.push_back(build_html_tree(now, dom["child"]));
		}
	}
	return now;
}
void show_tree();
node* simple_html_parser(std::string url)
{
	std::ifstream in(url);
	std::ostringstream buf;
	buf << in.rdbuf();
	std::string ss = buf.str();
	const char* html = ss.c_str();
	rapidjson::Document d;
	d.Parse(html);
#ifdef DEBUG_HTML
	std::cout << "html:" << std::endl << buf.str() << std::endl;
	assert(d.IsObject());
#endif
#ifdef DEBUG_HTML
	show_tree(root);
#endif
	return build_html_tree(nullptr, d);
}
node* html_init(std::string url)
{
	return simple_html_parser(url);
}
