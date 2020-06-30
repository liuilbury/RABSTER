//
// Created by liulizhang on 2020/6/30.
//

#include "bytecode/bytecode.h"
#include "bytecode/opcodes.h"
#include "select/propset.h"
#include "select/propget.h"
#include "utils/utils.h"

#include "select/properties/properties.h"
#include "select/properties/helpers.h"
css_error css__cascade_transition_property(uint32_t opv, css_style* style,
	css_select_state* state)
{
	uint16_t value = CSS_TRANSITION_PROPERTY_INHERIT;
	UNUSED(style);
	if (isInherit(opv) == false)
	{
		switch (getValue(opv))
		{
		case TRANSITION_PROPERTY_NONE:
			value = CSS_TRANSITION_PROPERTY_NONE;
			break;
		case TRANSITION_PROPERTY_ALL:
			value = CSS_TRANSITION_PROPERTY_ALL;
			break;
		case TRANSITION_PROPERTY_SET:
			value = CSS_TRANSITION_PROPERTY_SET;
			break;
		}
	}
	if (css__outranks_existing(getOpcode(opv), isImportant(opv), state,
		isInherit(opv)))
	{
		return set_transition_property(state->computed, value);
	}
	return CSS_OK;
}
css_error css__set_transition_property_from_hint(const css_hint* hint,
	css_computed_style* style)
{
	return set_transition_property(style, hint->status);
}

css_error css__initial_transition_property(css_select_state* state)
{
	return set_transition_property(state->computed,
		CSS_TRANSITION_PROPERTY_ALL);
}

css_error css__compose_transition_property(const css_computed_style* parent,
	const css_computed_style* child,
	css_computed_style* result)
{
	uint8_t type = get_transition_property(child);

	if (type == CSS_TRANSITION_PROPERTY_INHERIT)
	{
		type = get_transition_property(parent);
	}

	return set_transition_property(result, type);
}
