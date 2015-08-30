#include <vector>
#include "token.hpp"

Token::Token(Token::Type type, int line, int pos)
	: type(type)
	, line(line)
	, pos(pos)
	, characters(std::vector<char>())
{
}

Token::Token(Token::Type type, int line, int pos, char c)
	: Token(type, line, pos)
{
	addCharacter(c);
}

void
Token::addCharacter(char c)
{
	characters.push_back(c);
}

std::string
Token::getTypeName()
{
	switch(type) {
		case WORD:					return "WORD";
		case WHITESPACE:			return "WHITESPACE";
		case ROUND_BRACKET_OPEN:	return "ROUND_BRACKET_OPEN";
		case ROUND_BRACKET_CLOSE:	return "ROUND_BRACKET_CLOSE";
		case CURLY_BRACKET_OPEN:	return "CURLY_BRACKET_OPEN";
		case CURLY_BRACKET_CLOSE:	return "CURLY_BRACKET_CLOSE";
		case SEMICOLON:				return "SEMICOLON";
	}
}

std::string
Token::getContents()
{
	return std::string(characters.begin(), characters.end());
}
