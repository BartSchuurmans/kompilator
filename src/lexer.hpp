#include <iostream>
#include <vector>

#include "token.hpp"

enum class character_type {
	UPPER_ALPHA,
	LOWER_ALPHA,
	DIGIT,
	SINGLE_SYMBOL,
	MAYBE_MULTIPLE_SYMBOL,
	WHITESPACE,
	OTHER,
};

enum class state_type {
	START,
	WORD,
	DIGIT,
	MAYBE_MULTIPLE_SYMBOL,
};

struct lexer_state {
	std::istream *input;
	int line = 1;
	int pos = -1;
	state_type current_state = state_type::START;

public:
	lexer_state(std::istream *i)
	: input(i)
	{
	}

	bool get_character(char &c);
};

class tokenize_error : std::exception {
	int line;
	int pos;
	char c;
	std::string msg;

public:
	tokenize_error(int line, int pos, char c)
	: line(line)
	, pos(pos)
	, c(c)
	, msg("Invalid character '" + std::string(1, c) + "' at line " + std::to_string(line) + " on position " + std::to_string(pos))
	{
	}

	virtual char const *
	what() const noexcept
	{
		return msg.c_str();
	}
};

std::vector<token *> tokenize(std::istream &input);
