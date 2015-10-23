#pragma once

#include <string>
#include <vector>

#define KOMPILATOR_LEXEME_TYPES(def)	\
	def(UNKNOWN)	\
	def(WHITESPACE)	\
	def(WORD)	\
	def(NUMBER)	\
	def(SEMICOLON)	\
	def(COLON)	\
	def(COMMA)	\
	def(PLUS)	\
	def(MINUS)	\
	def(MULTIPLY)	\
	def(DIVIDE)	\
	def(MODULO)	\
	def(EQUALS)	\
	def(NOT_EQUALS)	\
	def(LESS_THAN)	\
	def(LESS_THAN_EQUALS)	\
	def(GREATER_THAN)	\
	def(GREATER_THAN_EQUALS)	\
	def(ROUND_BRACKET_OPEN)	\
	def(ROUND_BRACKET_CLOSE)	\
	def(CURLY_BRACKET_OPEN)	\
	def(CURLY_BRACKET_CLOSE)	\
	def(SQUARE_BRACKET_OPEN)	\
	def(SQUARE_BRACKET_CLOSE)	\
	def(AND)	\
	def(OR)	\
	def(IS)	\
	def(NOT)	\
	def(EMPTY_LIST)

enum class lexeme_type {
	#define DEFINE_ENUM_TYPE(v) v,
	KOMPILATOR_LEXEME_TYPES(DEFINE_ENUM_TYPE)
	#undef DEFINE_ENUM_TYPE
};

class lexeme
{
public:
	lexeme_type type = lexeme_type::UNKNOWN;
	std::vector<char> characters;
	int line;
	int pos;

	lexeme(int line, int pos);
	lexeme(int line, int pos, char c);

	void add_character(char c);
	std::string get_type_name() const;
	std::string get_contents() const;
};
