//
// Created by liulizhang on 2020/6/24.
//

#ifndef _HTMLCONTENT_H_
#define _HTMLCONTENT_H_
#include <libcss/libcss.h>
#include "DomNode.h"
#include <vector>
#include <queue>
#include "gumbo.h"
#define STYLESHEET_BASE        0    /* base style sheet */
#define STYLESHEET_QUIRKS    1    /* quirks mode stylesheet */
#define STYLESHEET_ADBLOCK    2    /* adblocking stylesheet */
#define STYLESHEET_USER        3    /* user stylesheet */
#define STYLESHEET_START    4    /* start of document stylesheets */
struct html_stylesheet {
	DomNode* node;
	css_stylesheet *sheet;
	bool modified;
	bool unused;
};
class HtmlContent
{
 private:
	int stylesheet_count;//样式表数量
	std::vector<html_stylesheet>stylesheets;//样式表数组
	css_select_ctx *select_ctx;//选择器
	css_media media;
	char* encoding;
	bool quirks;
	std::list<std::string>css_url;
	DomNode* build_html_tree(DomNode* fa, GumboNode* dom);
	DomNode* simple_html_parser(std::string url);
	css_stylesheet *create__style();
	css_stylesheet *create_inline_style(std::string inline_string);
	void html_css_new_stylesheets();
	void html_css_new_selection_context();
	void html_css_append_stylesheets(const char* url);
	void node_style_updata(DomNode* d,css_node_data_action mod);
 public:
	DomNode *root;
	void html_init(std::string url);
	void get_tree_style(DomNode* root);
	void get_node_style(DomNode* d,const css_computed_style *parent_style);
};

#endif //_HTMLCONTENT_H_
