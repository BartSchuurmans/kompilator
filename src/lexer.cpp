#include <vector>
#include <cassert>
#include "lexer.hpp"

character_type
get_character_type(char c)
{
	if(c >= 'A' && c <= 'Z') {
		return character_type::UPPER_ALPHA;
	}
	if(c >= 'a' && c <= 'z') {
		return character_type::LOWER_ALPHA;
	}
	if(c >= '0' && c <= '9') {
		return character_type::DIGIT;
	}
	switch(c) {
		case '(':
		case ')':
		case '{':
		case '}':
		case ':':
		case ';':
		case ']':
		case ',':
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
			return character_type::SINGLE_SYMBOL;
		case '=':
		case '<':
		case '>':
		case '[':
		case '&':
		case '|':
		case '!':
			return character_type::MAYBE_MULTIPLE_SYMBOL;
		case ' ':
		case '\t':
		case '\n':
			return character_type::WHITESPACE;
		default:
			return character_type::OTHER;
	}
}

bool
lexer_state::get_character(char &c)
{
	input->get(c);
	if(!input->good()) {
		return false;
	}

	// Update line/column
	if(c == '\n') {
		line++;
		pos = 0;
	} else {
		pos++;
	}

	return true;
}

std::vector<token *>
tokenize(std::istream &input)
{
	auto state = lexer_state(&input);
	auto output = std::vector<token *>();

	char c;
	token *tok = nullptr;
	while(state.get_character(c)) {

		character_type t = get_character_type(c);

		auto end_token = [&tok, &state]() {
			assert(tok->type != token_type::UNKNOWN);
			tok = nullptr;
			state.current_state = state_type::START;
		};

retry_character:
		if(state.current_state == state_type::START) {
			// Start new token with first character
			tok = new token(state.line, state.pos, c);
			output.push_back(tok);
		} else {
			assert(tok != nullptr);
		}
		switch(state.current_state) {
			case state_type::START:
				switch(t) {
					case character_type::LOWER_ALPHA:
					case character_type::UPPER_ALPHA:
						tok->type = token_type::WORD;
						state.current_state = state_type::WORD;
						break;
					case character_type::DIGIT:
						tok->type = token_type::NUMBER;
						state.current_state = state_type::DIGIT;
						break;
					case character_type::MAYBE_MULTIPLE_SYMBOL:
						state.current_state = state_type::MAYBE_MULTIPLE_SYMBOL;
						break;
					case character_type::SINGLE_SYMBOL:
						switch(c) {
							case '(': tok->type = token_type::ROUND_BRACKET_OPEN; break;
							case ')': tok->type = token_type::ROUND_BRACKET_CLOSE; break;
							case '{': tok->type = token_type::CURLY_BRACKET_OPEN; break;
							case '}': tok->type = token_type::CURLY_BRACKET_CLOSE; break;
							case ':': tok->type = token_type::COLON; break;
							case ';': tok->type = token_type::SEMICOLON; break;
							case ']': tok->type = token_type::SQUARE_BRACKET_CLOSE; break;
							case ',': tok->type = token_type::COMMA; break;
							case '+': tok->type = token_type::PLUS; break;
							case '-': tok->type = token_type::MINUS; break;
							case '*': tok->type = token_type::MULTIPLY; break;
							case '/': tok->type = token_type::DIVIDE; break;
							case '%': tok->type = token_type::MODULO; break;
							default:
								throw tokenize_error(state.line, state.pos, c);
						}
						end_token();
						break;
					case character_type::WHITESPACE:
						tok->type = token_type::WHITESPACE;
						state.current_state = state_type::WHITESPACE;
						break;
					case character_type::OTHER:
						throw tokenize_error(state.line, state.pos, c);
				}
				break;
			case state_type::WORD:
				switch(t) {
					case character_type::LOWER_ALPHA:
					case character_type::UPPER_ALPHA:
					case character_type::DIGIT:
						tok->add_character(c);
						break;
					default:
						end_token();
						goto retry_character;
				}
				break;
			case state_type::DIGIT:
				switch(t) {
					case character_type::DIGIT:
						tok->add_character(c);
						break;
					default:
						end_token();
						goto retry_character;
				}
				break;
			case state_type::MAYBE_MULTIPLE_SYMBOL:
				assert(!tok->characters.empty());
				switch(tok->characters.front()) {
					case '<':
						if(c == '=') {
							tok->type = token_type::LESS_THAN_EQUALS;
							tok->add_character(c);
							end_token();
						} else {
							tok->type = token_type::LESS_THAN;
							end_token();
							goto retry_character;
						}
						break;
					case '>':
						if(c == '=') {
							tok->type = token_type::GREATER_THAN_EQUALS;
							tok->add_character(c);
							end_token();
						} else {
							tok->type = token_type::GREATER_THAN;
							end_token();
							goto retry_character;
						}
						break;
					case '&':
						if(c == '&') {
							tok->type = token_type::AND;
							tok->add_character(c);
							end_token();
						} else {
							throw tokenize_error(state.line, state.pos, c);
						}
						break;
					case '|':
						if(c == '|') {
							tok->type = token_type::OR;
							tok->add_character(c);
							end_token();
						} else {
							throw tokenize_error(state.line, state.pos, c);
						}
						break;
					case '!':
						if(c == '=') {
							tok->type = token_type::NOT_EQUALS;
							tok->add_character(c);
							end_token();
						} else {
							tok->type = token_type::NOT;
							end_token();
							goto retry_character;
						}
						break;
					case '=':
						if(c == '=') {
							tok->type = token_type::EQUALS;
							tok->add_character(c);
							end_token();
						} else {
							tok->type = token_type::IS;
							end_token();
							goto retry_character;
						}
						break;
					case '[':
						if(c == ']') {
							tok->type = token_type::EMPTY_LIST;
							tok->add_character(c);
							end_token();
						} else {
							tok->type = token_type::SQUARE_BRACKET_OPEN;
							end_token();
							goto retry_character;
						}
						break;
					default:
						end_token();
						goto retry_character;
				}
				break;
			case state_type::WHITESPACE:
				switch(t) {
					case character_type::WHITESPACE:
						tok->add_character(c);
						break;
					default:
						end_token();
						goto retry_character;
				}
				break;
		}
	}

	return output;
}
