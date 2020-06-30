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
css_error css__cascade_transition_timing_function(uint32_t opv, css_style* style,
	css_select_state* state)
{
	uint16_t value = CSS_TRANSITION_TIMING_FUNCTION_INHERIT;
	UNUSED(style);
	if (isInherit(opv) == false)
	{
		switch (getValue(opv))
		{
		case TRANSITION_TIMING_FUNCTION_EASE:
			value = CSS_TRANSITION_TIMING_FUNCTION_EASE;
			break;
		case TRANSITION_TIMING_FUNCTION_EASE_IN:
			value = CSS_TRANSITION_TIMING_FUNCTION_EASE_IN;
			break;
		case TRANSITION_TIMING_FUNCTION_EASE_OUT:
			value = CSS_TRANSITION_TIMING_FUNCTION_EASE_OUT;
			break;
		case TRANSITION_TIMING_FUNCTION_EASE_IN_OUT:
			value = CSS_TRANSITION_TIMING_FUNCTION_EASE_IN_OUT;
			break;
		}
	}
	if (css__outranks_existing(getOpcode(opv), isImportant(opv), state,
		isInherit(opv)))
	{
		return set_transition_timing_function(state->computed, value);
	}
	return CSS_OK;
}
css_error css__set_transition_timing_function_from_hint(const css_hint* hint,
	css_computed_style* style)
{
	return set_transition_timing_function(style, hint->status);
}

css_error css__initial_transition_timing_function(css_select_state* state)
{
	return set_transition_timing_function(state->computed,
		CSS_TRANSITION_PROPERTY_ALL);
}

css_error css__compose_transition_timing_function(const css_computed_style* parent,
	const css_computed_style* child,
	css_computed_style* result)
{
	uint8_t type = get_transition_timing_function(child);

	if (type == CSS_TRANSITION_TIMING_FUNCTION_INHERIT)
	{
		type = get_transition_timing_function(parent);
	}

	return set_transition_timing_function(result, type);
}