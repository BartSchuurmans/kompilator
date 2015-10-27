#include "evaluator.hpp"

std::vector<token>
evaluate(std::vector<lexeme> const &lexemes)
{
	auto tokens = std::vector<token>();

	for(auto const &lme : lexemes) {
		switch(lme.type) {
			case lexeme_type::WORD:
			{
				auto contents = lme.get_contents();
				if(contents == "Int") {
					tokens.emplace_back(token_type::TYPE_INT, &lme);
				} else if(contents == "Bool") {
					tokens.emplace_back(token_type::TYPE_BOOL, &lme);
				} else if(contents == "Void") {
					tokens.emplace_back(token_type::TYPE_VOID, &lme);
				} else if(contents == "if") {
					tokens.emplace_back(token_type::IF, &lme);
				} else if(contents == "else") {
					tokens.emplace_back(token_type::ELSE, &lme);
				} else if(contents == "while") {
					tokens.emplace_back(token_type::WHILE, &lme);
				} else if(contents == "return") {
					tokens.emplace_back(token_type::RETURN, &lme);
				} else if(contents == "True") {
					tokens.emplace_back(token_type::TRUE, &lme);
				} else if(contents == "False") {
					tokens.emplace_back(token_type::FALSE, &lme);
				} else {
					tokens.emplace_back(token_type::IDENTIFIER, &lme);
				}
				break;
			}

			case lexeme_type::NUMBER:
				tokens.emplace_back(token_type::INTEGER, &lme);
				break;

			#define DEFINE_IDENTICAL_EMPTY_TOKEN_CASE(t) \
			case lexeme_type::t: \
				tokens.emplace_back(token_type::t, &lme); \
				break;
			KOMPILATOR_IDENTICAL_EMPTY_TOKEN_TYPES(DEFINE_IDENTICAL_EMPTY_TOKEN_CASE)
			#undef DEFINE_IDENTICAL_EMPTY_TOKEN_CASE

			case lexeme_type::WHITESPACE:
				tokens.emplace_back(token_type::WHITESPACE, &lme);
				break;

			case lexeme_type::SINGLE_LINE_COMMENT:
			case lexeme_type::MULTI_LINE_COMMENT:
				tokens.emplace_back(token_type::COMMENT, &lme);
				break;

			case lexeme_type::UNKNOWN:
				throw evaluator_error(lme, "UNKNOWN lexeme passed to evaluator");
		}
	}

	return tokens;
}
