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
css_error css__cascade_transition_duration(uint32_t opv, css_style *style,
	css_select_state *state)
{
	return css__cascade_length_auto(opv, style, state, set_transition_duration);
}

css_error css__set_transition_duration_from_hint(const css_hint *hint,
	css_computed_style *style)
{
	return set_transition_duration(style, hint->status,hint->data.length.value, hint->data.length.unit);
}

css_error css__initial_transition_duration(css_select_state *state)
{
	return set_transition_duration(state->computed, CSS_TRANSITION_DURATION_AUTO, 0, CSS_UNIT_S);
}

css_error css__compose_transition_duration(const css_computed_style *parent,
	const css_computed_style *child,
	css_computed_style *result)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_S;
	uint8_t type = get_transition_duration(child, &length, &unit);

	if (type == CSS_TRANSITION_DURATION_INHERIT) {
		type = get_transition_duration(parent, &length, &unit);
	}

	return set_transition_duration(result, type, length, unit);
}