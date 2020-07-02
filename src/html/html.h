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
#include "gumbo.h"
std::queue<std::string> URL;
const YGConfigRef config = YGConfigNew();
node* build_html_tree(node* fa, GumboNode* dom)
{
	if (dom->type != GUMBO_NODE_ELEMENT)
	{
		return nullptr;
	}
	GumboAttribute* attr;
	node* now = new node();
	now->_parent = fa;
	GumboVector* children = &dom->v.element.children;
	node* n = nullptr, * _last_child = nullptr;
	if (dom->v.element.tag == GUMBO_TAG_STYLE)
	{
		return nullptr;
	}
	else if (dom->v.element.tag == GUMBO_TAG_LINK)
	{
		attr = gumbo_get_attribute(&dom->v.element.attributes, "href");
		URL.push(attr->value);
		return nullptr;
	}
	else
	{
#ifdef DEBUG_HTML
		printf("%s\n",gumbo_normalized_tagname(dom->v.element.tag));
#endif
		now->ygnode = YGNodeNewWithConfig(config);
		now->htmlnode = dom;
		const char* name;
		if (dom->v.element.tag != GUMBO_TAG_UNKNOWN)
			name = gumbo_normalized_tagname(dom->v.element.tag);
		else
			name = "UNKNOWN";
		now->real_name = name;
		lwc_intern_string(name, strlen(name), &now->name);
		if ((attr = gumbo_get_attribute(&dom->v.element.attributes, "class")))
		{
			char* class_name = std::strtok(const_cast<char*>(attr->value), " ");
			while (class_name != nullptr)
			{
				now->classes = static_cast<lwc_string**>(realloc(now->classes,
					(now->n_classes + 1) * sizeof(lwc_string*)));
				lwc_intern_string(class_name, strlen(class_name), &now->classes[now->n_classes]);
				class_name = strtok(NULL, " ");
				now->n_classes++;
			}
		}
		if ((attr = gumbo_get_attribute(&dom->v.element.attributes, "style")))
		{
			now->_inline_style = attr->value;
		}
		if ((attr = gumbo_get_attribute(&dom->v.element.attributes, "id")))
		{
			lwc_intern_string(attr->value, strlen(attr->value), &now->Id);
		}
		int index = 0;
		for (int i = 0; i < children->length; ++i)
		{
			n = build_html_tree(now, static_cast<GumboNode*>(children->data[i]));
			if (n != nullptr)
			{
				now->_children.push_back(n);
				YGNodeInsertChild(now->ygnode, n->ygnode, index++);
				n->_prev = _last_child;
				if (_last_child != nullptr)
					_last_child->_next = n;
				_last_child = n;
			}
		}
	}
	return now;
}
void show_tree();
node* simple_html_parser(std::string url)
{
	std::ifstream in(url, std::ios::in | std::ios::binary);
	std::string contents;
	in.seekg(0, std::ios::end);
	contents.resize(in.tellg());
	in.seekg(0, std::ios::beg);
	in.read(&contents[0], contents.size());
	in.close();
	GumboOutput* output = gumbo_parse_with_options(&kGumboDefaultOptions, contents.data(), contents.length());
#ifdef DEBUG_HTML
	std::cout << "html:" << std::endl << contents << std::endl;
	assert(d.IsObject());
#endif
#ifdef DEBUG_HTML
	show_tree(root);
#endif
	return build_html_tree(nullptr, output->root);
}
node* html_init(std::string url)
{
	node* root = simple_html_parser(url);
	while (!URL.empty())
	{
		root->url.push_back(URL.front());
		URL.pop();
	}
	return root;
}
