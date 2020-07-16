//
// Created by liulizhang on 2020/6/24.
//

#include <fstream>
#include <sstream>
#include <string.h>
#include <queue>
#include "HtmlContent.h"
#include "Handle.h"
static css_error resolve_url(void* pw,
	const char* base, lwc_string* rel, lwc_string** abs)
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
	htmlsheet.sheet = nullptr;
	html_css_append_stylesheets("F:\\opengl\\RABSTER\\resources\\default.css");//STYLESHEET_BASE
	stylesheets.push_back(htmlsheet);//STYLESHEET_QUIRKS
	stylesheets.push_back(htmlsheet);//STYLESHEET_ADBLOCK
	stylesheets.push_back(htmlsheet);//STYLESHEET_USER
	stylesheet_count = STYLESHEET_START;
}
uint8_t* buf;
void HtmlContent::html_css_append_stylesheets(const char* url)
{
	css_stylesheet* sheet = create__style();
	std::ifstream in(url);
	std::ostringstream buf;
	buf << in.rdbuf();
	std::string css = buf.str();
	const char* csss = css.c_str();
	in.close();
	css_stylesheet_append_data(sheet, (const uint8_t*)csss, css.size());
	css_stylesheet_data_done(sheet);
	html_stylesheet htmlsheet;
	htmlsheet.sheet = sheet;
	htmlsheet.unused = true;
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
css_stylesheet* HtmlContent::create_inline_style(std::string inline_string)
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
void show(DomNode* d)
{
	css_color color_shade;
	css_computed_color(d->_final_style->styles[CSS_PSEUDO_ELEMENT_NONE], &color_shade);
	printf("color of %s is %x\n", d->real_name.c_str(), color_shade);
	char buf[8];
	sprintf(buf, "%x", color_shade);
	for (int i = 0; i < 4; i++)
	{
		uint32_t x;
		sscanf(buf + 2 * i, "%2x", &x);
		printf("%d\n", x);
	}
}
void HtmlContent::get_node_style(DomNode* d, const css_computed_style* parent_style)
{
	css_stylesheet* inline_style = nullptr;
	css_computed_style* composed;
	if (!d->_inline_style.empty())
	{
		inline_style = create_inline_style(d->_inline_style);
	}
	css_select_results* styles;
	this->media.type = CSS_MEDIA_SCREEN;
	css_select_style(this->select_ctx, d, &this->media, inline_style,
		&selection_handler, this, &styles);
	d->_style = styles;
	if (parent_style != nullptr)
	{
		css_computed_style_compose(parent_style, styles->styles[CSS_PSEUDO_ELEMENT_NONE], compute_font_size, this, &composed);
		css_computed_style_destroy(styles->styles[CSS_PSEUDO_ELEMENT_NONE]);
		styles->styles[CSS_PSEUDO_ELEMENT_NONE] = composed;
	}
	d->_final_style = styles;
	//show(d);
}
void HtmlContent::get_tree_style(DomNode* d)
{
	const css_computed_style* parent_style = nullptr;
	if (d->_parent != nullptr)
	{
		d->hover = d->_parent->hover;
		parent_style = d->_parent->_final_style->styles[CSS_PSEUDO_ELEMENT_NONE];
	}
	get_node_style(d, parent_style);
	for (auto i:d->_children)
	{
		get_tree_style(i);
	}
}
void HtmlContent::node_style_updata(DomNode* d, css_node_data_action mod)
{
	css_libcss_node_data_handler(&selection_handler, mod, this, d, NULL, d->node_data);
	for (auto i:d->_children)
	{
		node_style_updata(i, CSS_NODE_ANCESTORS_MODIFIED);
	}
}
DomNode* HtmlContent::build_html_tree(DomNode* fa, GumboNode* dom)
{
	DomNode* now = new DomNode();
	now->_parent = fa;
	now->element = dom;
	if(dom->type==GUMBO_NODE_TEXT){
		const char* name;
		name="text";

		now->real_name = name;
		lwc_intern_string(name, strlen(name), &now->name);
		now->text=dom->v.text.text;
		return now;
	}
	GumboAttribute* attr;

	GumboVector* children = &dom->v.element.children;
	DomNode* n = nullptr, * _last_child = nullptr;
	if (dom->v.element.tag == GUMBO_TAG_STYLE)
	{
		std::string oo = "style(" + std::to_string(css_url.size()) + ").css";
		css_url.push_back(oo);
		std::ofstream fs(R"(F:\opengl\RABSTER\resources\)" + oo);
		GumboNode* data = static_cast<GumboNode*>(children->data[0]);
		fs << std::string(data->v.text.text);
		fs.close();
		return nullptr;
	}
	else if (dom->v.element.tag == GUMBO_TAG_LINK)
	{
		attr = gumbo_get_attribute(&dom->v.element.attributes, "href");
		css_url.push_back(attr->value);
		return nullptr;
	}
	else
	{
#ifdef DEBUG_HTML
		printf("%s\n",gumbo_normalized_tagname(dom->v.element.tag));
#endif
		const char* name;
		if (dom->v.element.tag != GUMBO_TAG_UNKNOWN)
			name = gumbo_normalized_tagname(dom->v.element.tag);
		else
			name = "UNKNOWN";
		now->real_name = name;
		lwc_intern_string(name, strlen(name), &now->name);
		if ((attr = gumbo_get_attribute(&dom->v.element.attributes, "class")))
		{
			char* class_name = strtok(const_cast<char*>(attr->value), " ");
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
		for (int i = 0; i < children->length; ++i)
		{
			GumboNode* new_dom = static_cast<GumboNode*>(children->data[i]);
			if (new_dom->type == GUMBO_NODE_ELEMENT||new_dom->type==GUMBO_NODE_TEXT)
			{
				n = build_html_tree(now, new_dom);
				if(n!= nullptr)
				{
					now->_children.push_back(n);
					n->_prev = _last_child;
					if (_last_child != nullptr)
						_last_child->_next = n;
					_last_child = n;
				}
			}
		}
	}
	return now;
}
DomNode* HtmlContent::simple_html_parser(std::string url)
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
void HtmlContent::html_init(std::string url)
{
	root = simple_html_parser(url);
	html_css_new_stylesheets();
	for (auto i:css_url)
	{
		std::string str = R"(F:\opengl\RABSTER\resources\)" + i;
		html_css_append_stylesheets(str.data());
	}
	html_css_new_selection_context();
	get_tree_style(root);
}



