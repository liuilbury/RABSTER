//
// Created by liulizhang on 2020/6/24.
//
#include "html/html.h"
int main(){
	const YGConfigRef config = YGConfigNew();
	const YGNodeRef YGroot = YGNodeNewWithConfig(config);

	node* root=html_init(R"(F:\opengl\glfw\test.html)");
	HtmlContent* html_ctx=new HtmlContent;
	html_ctx->html_css_new_stylesheets();
	html_ctx->html_css_append_stylesheets(R"(F:\opengl\glfw\test.css)");
	html_ctx->html_css_new_selection_context();
	html_ctx->get_tree_style(root);
}