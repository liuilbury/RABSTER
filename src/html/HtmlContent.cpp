//
// Created by liulizhang on 2020/6/24.
//

#include <fstream>
#include <sstream>
#include "HtmlContent.h"
#include "Handle.h"
static css_error resolve_url(void *pw,
	const char *base, lwc_string *rel, lwc_string **abs)
{
	UNUSED(pw);
	UNUSED(base);

	/* About as useless as possible */
	*abs = lwc_string_ref(rel);

	return CSS_OK;
}
void HtmlContent::html_css_new_stylesheets()
{
	html_stylesheet htmlsheet;
	htmlsheet.sheet= nullptr;
	stylesheets.push_back(htmlsheet);//STYLESHEET_BASE
	stylesheets.push_back(htmlsheet);//STYLESHEET_QUIRKS
	stylesheets.push_back(htmlsheet);//STYLESHEET_ADBLOCK
	stylesheets.push_back(htmlsheet);//STYLESHEET_USER
	stylesheet_count = STYLESHEET_START;
}
uint8_t *buf;
void HtmlContent::html_css_append_stylesheets(const char* url)
{
	css_stylesheet* sheet = create__style();
	std::ifstream in(url);
	std::ostringstream buf;
	buf << in.rdbuf();
	std::string css=buf.str();
	const char *csss=css.c_str();
	in.close();
	css_stylesheet_append_data(sheet,(const uint8_t*)csss,css.size());
	css_stylesheet_data_done(sheet);
	html_stylesheet htmlsheet;
	htmlsheet.sheet=sheet;
	htmlsheet.unused= true;
	stylesheets.push_back(htmlsheet);
	++stylesheet_count;
}
void HtmlContent::html_css_new_selection_context()
{
	css_select_ctx_create(&select_ctx);
	for (int i = 0; i < stylesheet_count; i++)
	{
		if (stylesheets[i].unused && stylesheets[i].sheet != nullptr)
		{
			css_select_ctx_append_sheet(select_ctx, stylesheets[i].sheet, CSS_ORIGIN_AUTHOR, nullptr);
		}
	}
}
css_stylesheet* HtmlContent::create__style()
{
	css_stylesheet* sheet;
	css_stylesheet_params params;
	params.params_version = CSS_STYLESHEET_PARAMS_VERSION_1;
	params.level = CSS_LEVEL_DEFAULT;
	params.charset = "UTF-8";
	params.url = "foo";
	params.title = "foo";
	params.allow_quirks = false;
	params.inline_style = false;
	params.resolve = resolve_url;
	params.resolve_pw = nullptr;
	params.import = nullptr;
	params.import_pw = nullptr;
	params.color = nullptr;
	params.color_pw = nullptr;
	params.font = nullptr;
	params.font_pw = nullptr;
	assert(css_stylesheet_create(&params, &sheet) == CSS_OK);
	return sheet;
}
css_stylesheet* HtmlContent::create_inline_style(string inline_string)
{
	css_stylesheet* sheet;
	css_stylesheet_params params;
	params.params_version = CSS_STYLESHEET_PARAMS_VERSION_1;
	params.level = CSS_LEVEL_DEFAULT;
	params.charset = "UTF-8";
	params.url = "foo";
	params.title = "foo";
	params.allow_quirks = false;
	params.inline_style = true;
	params.resolve = resolve_url;
	params.resolve_pw = nullptr;
	params.import = nullptr;
	params.import_pw = nullptr;
	params.color = nullptr;
	params.color_pw = nullptr;
	params.font = nullptr;
	params.font_pw = nullptr;
	css_stylesheet_create(&params, &sheet);
	const char* data = inline_string.c_str();
	css_stylesheet_append_data(sheet, (const uint8_t*)data, inline_string.size());
	css_stylesheet_data_done(sheet);
	return sheet;
}
void show(node* d){
	css_color color_shade;
	css_computed_color(d->_final_style->styles[CSS_PSEUDO_ELEMENT_NONE],&color_shade);
	printf("color of %s is %x\n",d->real_name.c_str() , color_shade);
	char buf[8];
	sprintf(buf,"%x",color_shade);
	for(int i=0;i<4;i++)
	{
		uint32_t x;
		sscanf(buf+2*i,"%2x",&x);
		printf("%d\n",x);
	}
}
void HtmlContent::get_node_style(node* d, const css_computed_style* parent_style)
{
	css_stylesheet* inline_style = nullptr;
	css_computed_style* composed;
	int flag=0;
	if (!d->_inline_style.empty())
	{
		inline_style = create_inline_style(d->_inline_style);
	}
	css_select_results* styles;
	this->media.type = CSS_MEDIA_SCREEN;
	css_select_style(this->select_ctx, d, &this->media, inline_style,
		&selection_handler, this, &styles);
	d->_style=styles;
	if (parent_style != nullptr)
	{
		css_computed_style_compose(parent_style, styles->styles[CSS_PSEUDO_ELEMENT_NONE], compute_font_size, this, &composed);
		css_computed_style_destroy(styles->styles[CSS_PSEUDO_ELEMENT_NONE]);
		styles->styles[CSS_PSEUDO_ELEMENT_NONE] = composed;
	}
	d->_final_style=styles;
	//show(d);
}

void HtmlContent::get_tree_style(node* d)
{
	const css_computed_style* parent_style = nullptr;
	if (d->_parent != nullptr)
	{
		parent_style = d->_parent->_final_style->styles[CSS_PSEUDO_ELEMENT_NONE];
	}
	get_node_style(d, parent_style);
	for (auto i:d->_children)
	{
		get_tree_style(i);
	}
}
void HtmlContent::node_style_updata(node* d,css_node_data_action mod)
{
	css_libcss_node_data_handler(&selection_handler, mod,this, d, NULL, d->node_data);
	for(auto i:d->_children){
		node_style_updata(i,CSS_NODE_ANCESTORS_MODIFIED);
	}
}



