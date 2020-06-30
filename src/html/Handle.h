//
// Created by 53247 on 2020/6/27.
//

#ifndef GLFW_HANDLE_H
#define GLFW_HANDLE_H
#include "libcss/libcss.h"
#define UNUSED(x) ((x) = (x))

css_error node_name(void* pw, void* n, css_qname* qname)
{
	node* node = static_cast<class node*>(n);

	UNUSED(pw);

	qname->name = lwc_string_ref(node->name);

	return CSS_OK;
}

css_error node_classes(void* pw, void* n,
	lwc_string*** classes, uint32_t* n_classes)
{
	node* node = static_cast<class node*>(n);
	UNUSED(pw);
	unsigned int i;
	*classes = node->classes;
	*n_classes = node->n_classes;
	for (i = 0; i < *n_classes; i++)
		(*classes)[i] = lwc_string_ref(node->classes[i]);
	return CSS_OK;
}

css_error node_id(void* pw, void* n, lwc_string** id)
{
	node* node = static_cast<class node*>(n);
	UNUSED(pw);
	*id = node->Id;
	return CSS_OK;
}

css_error named_ancestor_node(void* pw, void* n, const css_qname* qname, void** ancestor)
{
	node* node = static_cast<class node*>(n);
	for (node = node->_parent; node != nullptr; node = node->_parent)
	{
		bool match = false;
		assert(lwc_string_caseless_isequal(
			qname->name, node->name,
			&match) == lwc_error_ok);
		if (match)
			break;
	}
	UNUSED(pw);
	*ancestor = (void*)node;
	return CSS_OK;
}

css_error named_parent_node(void* pw, void* n, const css_qname* qname, void** parent)
{
	node* node = static_cast<class node*>(n);
	UNUSED(pw);
	if (node->_parent != nullptr)
	{
		bool match = false;
		assert(lwc_string_caseless_isequal(
			qname->name, node->_parent->name, &match) ==
			   lwc_error_ok);
		if (match)
			*parent = (void*)node->_parent;
	}
	return CSS_OK;
}

css_error named_generic_sibling_node(void* pw, void* n, const css_qname* qname, void** sibling)
{
	node* node = static_cast<class node*>(n);
	for (node = node->_prev; node != nullptr; node = node->_prev)
	{
		bool match = false;
		assert(lwc_string_caseless_isequal(
			qname->name, node->name,
			&match) == lwc_error_ok);
		if (match)
			break;
	}
	UNUSED(pw);
	*sibling = (void*)node;
	return CSS_OK;
}

css_error named_sibling_node(void* pw, void* n, const css_qname* qname, void** sibling)
{
	node* node = static_cast<class node*>(n);
	*sibling = nullptr;
	UNUSED(pw);
	if (node->_prev != nullptr)
	{
		bool match = false;
		assert(lwc_string_caseless_isequal(
			qname->name, node->_prev->name, &match) ==
			   lwc_error_ok);
		if (match == true)
			*sibling = (void*)node->_prev;
	}
	return CSS_OK;
}

css_error parent_node(void* pw, void* n, void** parent)
{
	node* node = static_cast<class node*>(n);
	*parent = (void*)node->_parent;
	UNUSED(pw);
	return CSS_OK;
}

css_error sibling_node(void* pw, void* n, void** sibling)
{
	node* node = static_cast<class node*>(n);
	*sibling = (void*)node->_prev;
	UNUSED(pw);
	return CSS_OK;
}

css_error node_has_name(void* pw, void* n, const css_qname* qname, bool* match)
{
	node* node = static_cast<class node*>(n);
	UNUSED(pw);
	if (lwc_string_length(qname->name) == 1 &&
		lwc_string_data(qname->name)[0] == '*')
		*match = true;
	else
		assert(lwc_string_caseless_isequal(node->name,
			qname->name, match) == lwc_error_ok);
	return CSS_OK;
}

css_error node_has_class(void* pw, void* n, lwc_string* name, bool* match)
{
	node* node = static_cast<class node*>(n);
	UNUSED(pw);
	UNUSED(name);
	*match = (node->n_classes != 0);
	return CSS_OK;
}

css_error node_has_id(void* pw, void* n,
	lwc_string* name,
	bool* match)
{
	node* node = static_cast<class node*>(n);
	UNUSED(pw);
	UNUSED(name);
	*match = (node->Id != nullptr);
	return CSS_OK;
}

css_error node_has_attribute(void* pw, void* n,
	const css_qname* qname,
	bool* match)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(qname);
	*match = false;
	return CSS_OK;
}

css_error node_has_attribute_equal(void* pw, void* n,
	const css_qname* qname,
	lwc_string* value,
	bool* match)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(qname);
	UNUSED(value);
	*match = false;
	return CSS_OK;
}

css_error node_has_attribute_dashmatch(void* pw, void* n,
	const css_qname* qname,
	lwc_string* value,
	bool* match)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(qname);
	UNUSED(value);
	*match = false;
	return CSS_OK;
}

css_error node_has_attribute_includes(void* pw, void* n,
	const css_qname* qname,
	lwc_string* value,
	bool* match)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(qname);
	UNUSED(value);
	*match = false;
	return CSS_OK;
}

css_error node_has_attribute_prefix(void* pw, void* n,
	const css_qname* qname,
	lwc_string* value,
	bool* match)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(qname);
	UNUSED(value);
	*match = false;
	return CSS_OK;
}

css_error node_has_attribute_suffix(void* pw, void* n,
	const css_qname* qname,
	lwc_string* value,
	bool* match)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(qname);
	UNUSED(value);
	*match = false;
	return CSS_OK;
}

css_error node_has_attribute_substring(void* pw, void* n,
	const css_qname* qname,
	lwc_string* value,
	bool* match)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(qname);
	UNUSED(value);
	*match = false;
	return CSS_OK;
}

css_error node_is_root(void* pw, void* n, bool* match)
{
	UNUSED(pw);
	node* node = static_cast<class node*>(n);
	*match = (node->_parent == nullptr);
	return CSS_OK;
}

css_error node_count_siblings(void* pw, void* n,
	bool same_name, bool after, int32_t* count)
{
	int cnt = 0;
	bool match = false;
	node* node = static_cast<class node*>(n);
	lwc_string* name = node->name;
	UNUSED(pw);
	if (after)
	{
		while(node->_next!= nullptr){
			if(same_name){
				assert(lwc_string_caseless_isequal(
					name, node->_next->name, &match) ==
					   lwc_error_ok);

				if (match)
					cnt++;
			}else{
				cnt++;
			}
			node=node->_next;
		}
	}else{
		while(node->_prev!= nullptr){
			if(same_name){
				assert(lwc_string_caseless_isequal(
					name, node->_prev->name, &match) ==
					   lwc_error_ok);

				if (match)
					cnt++;
			}else{
				cnt++;
			}
			node=node->_prev;
		}
	}
	*count = cnt;
	return CSS_OK;
}

css_error node_is_empty(void* pw, void* n, bool* match)
{
	UNUSED(pw);
	node* node = static_cast<class node*>(n);
	*match = (node->_children.size() == 0);
	return CSS_OK;
}

css_error node_is_link(void* pw, void* n, bool* match)
{
	UNUSED(pw);
	UNUSED(n);
	*match = false;
	return CSS_OK;
}

css_error node_is_visited(void* pw, void* n, bool* match)
{
	UNUSED(pw);
	UNUSED(n);
	*match = false;
	return CSS_OK;
}

css_error node_is_hover(void* pw, void* n, bool* match)
{
	UNUSED(pw);
	UNUSED(n);
	*match = false;
	return CSS_OK;
}

css_error node_is_active(void* pw, void* n, bool* match)
{
	UNUSED(pw);
	UNUSED(n);
	*match = false;
	return CSS_OK;
}

css_error node_is_focus(void* pw, void* n, bool* match)
{
	UNUSED(pw);
	UNUSED(n);
	*match = false;
	return CSS_OK;
}

css_error node_is_enabled(void* pw, void* n, bool* match)
{
	UNUSED(pw);
	UNUSED(n);
	*match = false;
	return CSS_OK;
}

css_error node_is_disabled(void* pw, void* n, bool* match)
{
	UNUSED(pw);
	UNUSED(n);
	*match = false;
	return CSS_OK;
}

css_error node_is_checked(void* pw, void* n, bool* match)
{
	UNUSED(pw);
	UNUSED(n);
	*match = false;
	return CSS_OK;
}

css_error node_is_target(void* pw, void* n, bool* match)
{
	UNUSED(pw);
	UNUSED(n);
	*match = false;
	return CSS_OK;
}

css_error node_is_lang(void* pw, void* n,
	lwc_string* lang,
	bool* match)
{
	UNUSED(pw);
	UNUSED(n);
	UNUSED(lang);
	*match = false;
	return CSS_OK;
}

css_error node_presentational_hint(void* pw, void* node,
	uint32_t* nhints, css_hint** hints)
{
	UNUSED(pw);
	UNUSED(node);
	*nhints = 0;
	*hints = NULL;
	return CSS_OK;
}

css_error ua_default_for_property(void* pw, uint32_t property, css_hint* hint)
{
	UNUSED(pw);

	if (property == CSS_PROP_COLOR)
	{
		hint->data.color = 0x00000000;
		hint->status = CSS_COLOR_COLOR;
	}
	else if (property == CSS_PROP_FONT_FAMILY)
	{
		hint->data.strings = NULL;
		hint->status = CSS_FONT_FAMILY_SANS_SERIF;
	}
	else if (property == CSS_PROP_QUOTES)
	{
		/* Not exactly useful :) */
		hint->data.strings = NULL;
		hint->status = CSS_QUOTES_NONE;
	}
	else if (property == CSS_PROP_VOICE_FAMILY)
	{
		/** \todo Fix this when we have voice-family done */
		hint->data.strings = NULL;
		hint->status = 0;
	}
	else
	{
		return CSS_INVALID;
	}

	return CSS_OK;
}

css_error compute_font_size(void* pw, const css_hint* parent, css_hint* size)
{
	static css_hint_length sizes[] = {
		{ FLTTOFIX(6.75), CSS_UNIT_PT },
		{ FLTTOFIX(7.50), CSS_UNIT_PT },
		{ FLTTOFIX(9.75), CSS_UNIT_PT },
		{ FLTTOFIX(12.0), CSS_UNIT_PT },
		{ FLTTOFIX(13.5), CSS_UNIT_PT },
		{ FLTTOFIX(18.0), CSS_UNIT_PT },
		{ FLTTOFIX(24.0), CSS_UNIT_PT }
	};
	const css_hint_length* parent_size;

	UNUSED(pw);

	/* Grab parent size, defaulting to medium if none */
	if (parent == NULL)
	{
		parent_size = &sizes[CSS_FONT_SIZE_MEDIUM - 1];
	}
	else
	{
		assert(parent->status == CSS_FONT_SIZE_DIMENSION);
		assert(parent->data.length.unit != CSS_UNIT_EM);
		assert(parent->data.length.unit != CSS_UNIT_EX);
		parent_size = &parent->data.length;
	}

	assert(size->status != CSS_FONT_SIZE_INHERIT);

	if (size->status < CSS_FONT_SIZE_LARGER)
	{
		/* Keyword -- simple */
		size->data.length = sizes[size->status - 1];
	}
	else if (size->status == CSS_FONT_SIZE_LARGER)
	{
		/** \todo Step within table, if appropriate */
		size->data.length.value =
			FMUL(parent_size->value, FLTTOFIX(1.2));
		size->data.length.unit = parent_size->unit;
	}
	else if (size->status == CSS_FONT_SIZE_SMALLER)
	{
		/** \todo Step within table, if appropriate */
		size->data.length.value =
			FMUL(parent_size->value, FLTTOFIX(1.2));
		size->data.length.unit = parent_size->unit;
	}
	else if (size->data.length.unit == CSS_UNIT_EM ||
			 size->data.length.unit == CSS_UNIT_EX)
	{
		size->data.length.value =
			FMUL(size->data.length.value, parent_size->value);

		if (size->data.length.unit == CSS_UNIT_EX)
		{
			size->data.length.value = FMUL(size->data.length.value,
				FLTTOFIX(0.6));
		}

		size->data.length.unit = parent_size->unit;
	}
	else if (size->data.length.unit == CSS_UNIT_PCT)
	{
		size->data.length.value = FDIV(FMUL(size->data.length.value,
			parent_size->value), FLTTOFIX(100));
		size->data.length.unit = parent_size->unit;
	}

	size->status = CSS_FONT_SIZE_DIMENSION;

	return CSS_OK;
}

static css_error set_libcss_node_data(void* pw, void* n,
	void* libcss_node_data)
{
	node* node = static_cast<class node*>(n);
	UNUSED(pw);
	node->node_data = libcss_node_data;
	return CSS_OK;
}

static css_error get_libcss_node_data(void* pw, void* n,
	void** libcss_node_data)
{
	node* node = static_cast<class node*>(n);
	UNUSED(pw);
	*libcss_node_data = node->node_data;

	return CSS_OK;
}
static css_select_handler selection_handler = {
	CSS_SELECT_HANDLER_VERSION_1,
	node_name,
	node_classes,
	node_id,
	named_ancestor_node,
	named_parent_node,
	named_sibling_node,
	named_generic_sibling_node,
	parent_node,
	sibling_node,
	node_has_name,
	node_has_class,
	node_has_id,
	node_has_attribute,
	node_has_attribute_equal,
	node_has_attribute_dashmatch,
	node_has_attribute_includes,
	node_has_attribute_prefix,
	node_has_attribute_suffix,
	node_has_attribute_substring,
	node_is_root,
	node_count_siblings,
	node_is_empty,
	node_is_link,
	node_is_visited,
	node_is_hover,
	node_is_active,
	node_is_focus,
	node_is_enabled,
	node_is_disabled,
	node_is_checked,
	node_is_target,
	node_is_lang,
	node_presentational_hint,
	ua_default_for_property,
	compute_font_size,
	set_libcss_node_data,
	get_libcss_node_data
};

#endif //GLFW_HANDLE_H
