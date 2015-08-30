#include <iostream>
#include <vector>

#include "token.hpp"

class TokenizeError : std::exception {
	int line;
	int pos;
	char c;
	std::string msg;
public:
	TokenizeError(int line, int pos, char c)
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

std::vector<Token *> tokenize(std::istream &input);
