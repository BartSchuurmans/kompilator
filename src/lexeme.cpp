#include <vector>
#include "lexeme.hpp"

lexeme::lexeme(int line, int pos)
	: characters()
	, line(line)
	, pos(pos)
{
}

lexeme::lexeme(int line, int pos, char c)
	: lexeme(line, pos)
{
	add_character(c);
}

void
lexeme::add_character(char c)
{
	characters.push_back(c);
}

std::string
lexeme::get_contents()
{
	return std::string(characters.begin(), characters.end());
}

std::string
lexeme::get_type_name()
{
	switch(type) {
		#define DEFINE_SWITCH_CASE(v) case lexeme_type::v: return #v;
		KOMPILATOR_LEXEME_TYPES(DEFINE_SWITCH_CASE)
		#undef DEFINE_SWITCH_CASE
	}
}
