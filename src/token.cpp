#include <vector>
#include "token.hpp"

token::token(int line, int pos)
	: line(line)
	, pos(pos)
	, characters()
{
}

token::token(int line, int pos, char c)
	: token(line, pos)
{
	add_character(c);
}

void
token::add_character(char c)
{
	characters.push_back(c);
}

std::string
token::get_contents()
{
	return std::string(characters.begin(), characters.end());
}

std::string
token::get_type_name()
{
	switch(type) {
		#define DEFINE_SWITCH_CASE(v) case token_type::v: return #v;
		KOMPILATOR_TOKEN_TYPES(DEFINE_SWITCH_CASE)
		#undef DEFINE_SWITCH_CASE
	}
}
