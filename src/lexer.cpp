#include <vector>
#include "lexer.hpp"

enum CharacterType {
	OTHER,
	DIGIT,
	ALPHA,
	WHITESPACE,
	UNDERSCORE,
	ROUND_BRACKET_OPEN,
	ROUND_BRACKET_CLOSE,
	CURLY_BRACKET_OPEN,
	CURLY_BRACKET_CLOSE,
	SEMICOLON,
};

CharacterType
getCharacterType(char c)
{
	if(c >= '0' && c <= '9') {
		return CharacterType::DIGIT;
	}
	if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
		return CharacterType::ALPHA;
	}
	switch(c) {
		case '(':
			return CharacterType::ROUND_BRACKET_OPEN;
		case ')':
			return CharacterType::ROUND_BRACKET_CLOSE;
		case '{':
			return CharacterType::CURLY_BRACKET_OPEN;
		case '}':
			return CharacterType::CURLY_BRACKET_CLOSE;
		case ';':
			return CharacterType::SEMICOLON;
		case ' ':
		case '\t':
		case '\n':
			return CharacterType::WHITESPACE;
		default:
			return CharacterType::OTHER;
	}
}
	

std::vector<Token *>
tokenize(std::istream &input)
{
	auto output = std::vector<Token *>();

	int line = 1;
	int pos = -1;

	char c;
	Token *currentToken = nullptr;
	while(input.get(c)) {
		// Update line/column
		if(c == '\n') {
			line++;
			pos = 0;
		} else {
			pos++;
		}

		CharacterType type = getCharacterType(c);
		if(currentToken != nullptr) {
			// Character might continue current token
			switch(currentToken->type) {
				case Token::Type::WORD:
					switch(type) {
						case CharacterType::ALPHA:
						case CharacterType::DIGIT:
						case CharacterType::UNDERSCORE:
							currentToken->addCharacter(c);
							continue;
						default:
							// Doesn't match
							break;
					}
					break;
				case Token::Type::WHITESPACE:
					if(type == CharacterType::WHITESPACE) {
						currentToken->addCharacter(c);
						continue;
					}
					break;
				default:
					// Token can not be elongated
					break;
			}
			// Character could not be added to current token
			currentToken = nullptr;
		}

		// Start new token based on character type
		switch(type) {
			case CharacterType::ALPHA:
			case CharacterType::DIGIT:
				currentToken = new Token(Token::Type::WORD, line, pos, c);
				break;
			case CharacterType::WHITESPACE:
				currentToken = new Token(Token::Type::WHITESPACE, line, pos, c);
				break;
			case CharacterType::ROUND_BRACKET_OPEN:
				currentToken = new Token(Token::Type::ROUND_BRACKET_OPEN, line, pos, c);
				break;
			case CharacterType::ROUND_BRACKET_CLOSE:
				currentToken = new Token(Token::Type::ROUND_BRACKET_CLOSE, line, pos, c);
				break;
			case CharacterType::CURLY_BRACKET_OPEN:
				currentToken = new Token(Token::Type::CURLY_BRACKET_OPEN, line, pos, c);
				break;
			case CharacterType::CURLY_BRACKET_CLOSE:
				currentToken = new Token(Token::Type::CURLY_BRACKET_CLOSE, line, pos, c);
				break;
			case CharacterType::SEMICOLON:
				currentToken = new Token(Token::Type::SEMICOLON, line, pos, c);
				break;
			case CharacterType::OTHER:
			default:
				throw TokenizeError(line, pos, c);
		}
		output.push_back(currentToken);
	}

	return output;
}
