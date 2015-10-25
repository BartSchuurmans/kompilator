#include <cassert>

#include "token.hpp"

token_type
get_token_type_by_name(const std::string &name)
{
	try {
		return token_types_by_name.at(name);
	} catch(const std::out_of_range &e) {
		throw std::runtime_error("Non-existent token: " + name);
	}
}

token::token(token_type type, const lexeme *lme)
	: type(type)
	, lme(lme)
{
}

std::string
token::get_type_name() const
{
	switch(type) {
		#define DEFINE_SWITCH_CASE(v) case token_type::v: return #v;
		KOMPILATOR_TOKEN_TYPES(DEFINE_SWITCH_CASE)
		#undef DEFINE_SWITCH_CASE
	}
}

bool
token::has_contents() const
{
	switch(type) {
		case token_type::IDENTIFIER:
		case token_type::INTEGER:
			return true;
		default:
			return false;
	}
}

std::string
token::get_contents() const
{
	assert(has_contents());
	return lme->get_contents();
}
