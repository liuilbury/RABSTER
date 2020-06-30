//
// Created by liulizhang on 2020/6/30.
//
#include <assert.h>
#include <string.h>

#include "bytecode/bytecode.h"
#include "bytecode/opcodes.h"
#include "parse/properties/properties.h"
#include "parse/properties/utils.h"

///transition

css_error css__parse_transition(css_language* c,
	const parserutils_vector* vector, int* ctx,
	css_style* result)
{
	return CSS_OK;
}
css_error css__parse_transition_delay(css_language* c,
	const parserutils_vector* vector, int* ctx,
	css_style* result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token* token;
	bool match;

	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL)
	{
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	css_fixed time = 0;
	uint32_t unit = 0;
	*ctx = orig_ctx;
	error = css__parse_unit_specifier(c, vector, ctx, UNIT_S, &time, &unit);
	if (error != CSS_OK)
	{
		*ctx = orig_ctx;
		return error;
	}
	if (!(unit & UNIT_TIME))
	{
		*ctx = orig_ctx;
		return CSS_INVALID;
	}
	if (time < 0)
	{
		*ctx = orig_ctx;
		return CSS_INVALID;
	}
	error = css__stylesheet_style_appendOPV(result, CSS_PROP_TRANSITION_DELAY, 0, TRANSITION_DELAY_SET);
	if (error != CSS_OK)
	{
		*ctx = orig_ctx;
		return error;
	}
	error = css__stylesheet_style_vappend(result, 2, time, unit);
	if (error != CSS_OK)
		*ctx = orig_ctx;
	return error;
}
css_error css__parse_transition_duration(css_language* c,
	const parserutils_vector* vector, int* ctx,
	css_style* result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token* token;
	bool match;

	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL)
	{
		*ctx = orig_ctx;
		return CSS_INVALID;
	}

	css_fixed time = 0;
	uint32_t unit = 0;
	*ctx = orig_ctx;
	error = css__parse_unit_specifier(c, vector, ctx, UNIT_S, &time, &unit);
	if (error != CSS_OK)
	{
		*ctx = orig_ctx;
		return error;
	}
	if (!(unit & UNIT_TIME))
	{
		*ctx = orig_ctx;
		return CSS_INVALID;
	}
	if (time < 0)
	{
		*ctx = orig_ctx;
		return CSS_INVALID;
	}
	error = css__stylesheet_style_appendOPV(result, CSS_PROP_TRANSITION_DURATION, 0, TRANSITION_DURATION_SET);
	if (error != CSS_OK)
	{
		*ctx = orig_ctx;
		return error;
	}
	error = css__stylesheet_style_vappend(result, 2, time, unit);
	if (error != CSS_OK)
		*ctx = orig_ctx;
	return error;
}
css_error css__parse_transition_property(css_language* c,
	const parserutils_vector* vector, int* ctx,
	css_style* result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *token;
	bool match;

	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}
	if ((token->type == CSS_TOKEN_IDENT) && (lwc_string_caseless_isequal(token->idata, c->strings[NONE], &match) == lwc_error_ok && match)) {
		error = css__stylesheet_style_appendOPV(result, CSS_PROP_TRANSITION_PROPERTY, 0,TRANSITION_PROPERTY_NONE);
	}else if ((token->type == CSS_TOKEN_IDENT) && (lwc_string_caseless_isequal(token->idata, c->strings[ALL], &match) == lwc_error_ok && match)) {
		error = css__stylesheet_style_appendOPV(result, CSS_PROP_TRANSITION_PROPERTY, 0,TRANSITION_PROPERTY_ALL);
	}else{
		error=CSS_INVALID;
		printf("%s\n",lwc_string_data(token->idata));
		printf("no complete\n");
	}
	if (error != CSS_OK)
		*ctx = orig_ctx;
	return error;
}
css_error css__parse_transition_timing_function(css_language* c,
	const parserutils_vector* vector, int* ctx,
	css_style* result)
{
	int orig_ctx = *ctx;
	css_error error;
	const css_token *token;
	bool match;

	token = parserutils_vector_iterate(vector, ctx);
	if (token == NULL) {
		*ctx = orig_ctx;
		return CSS_INVALID;
	}
	if ((token->type == CSS_TOKEN_IDENT) && (lwc_string_caseless_isequal(token->idata, c->strings[LINEAR], &match) == lwc_error_ok && match)) {
		error = css__stylesheet_style_appendOPV(result, CSS_PROP_TRANSITION_TIMING_FUNCTION, 0,TRANSITION_TIMING_FUNCTION_LINEAR);
	}else if ((token->type == CSS_TOKEN_IDENT) && (lwc_string_caseless_isequal(token->idata, c->strings[EASE], &match) == lwc_error_ok && match)) {
		error = css__stylesheet_style_appendOPV(result, CSS_PROP_TRANSITION_TIMING_FUNCTION, 0,TRANSITION_TIMING_FUNCTION_EASE);
	}else if ((token->type == CSS_TOKEN_IDENT) && (lwc_string_caseless_isequal(token->idata, c->strings[EASE_IN], &match) == lwc_error_ok && match)) {
		error = css__stylesheet_style_appendOPV(result, CSS_PROP_TRANSITION_TIMING_FUNCTION, 0,TRANSITION_TIMING_FUNCTION_EASE_IN);
	}else if ((token->type == CSS_TOKEN_IDENT) && (lwc_string_caseless_isequal(token->idata, c->strings[EASE_OUT], &match) == lwc_error_ok && match)) {
		error = css__stylesheet_style_appendOPV(result, CSS_PROP_TRANSITION_TIMING_FUNCTION, 0,TRANSITION_TIMING_FUNCTION_EASE_OUT);
	}else if ((token->type == CSS_TOKEN_IDENT) && (lwc_string_caseless_isequal(token->idata, c->strings[EASE_IN_OUT], &match) == lwc_error_ok && match)) {
		error = css__stylesheet_style_appendOPV(result, CSS_PROP_TRANSITION_TIMING_FUNCTION, 0,TRANSITION_TIMING_FUNCTION_EASE_IN_OUT);
	}else {
		error=CSS_INVALID;
		printf("%d\n",token->type);
		printf("no complete\n");
	}
	if (error != CSS_OK)
		*ctx = orig_ctx;
	return error;
}