#ifndef H_EVALUATOR
#define H_EVALUATOR

#include <iostream>
#include <vector>

#include "token.hpp"

class evaluator_error : std::exception {
	const lexeme lme;
	std::string msg;

public:
	evaluator_error(lexeme const &lme, std::string const &error)
	: lme(lme)
	, msg("Error evaluating " + lme.get_type_name() + " token at line " + std::to_string(lme.line) + " on position "+ std::to_string(lme.pos) + ": " + error)
	{
	}

	virtual char const *
	what() const noexcept
	{
		return msg.c_str();
	}
};

std::vector<token> evaluate(std::vector<lexeme> const &lexemes);

#endif	// H_EVALUATOR
