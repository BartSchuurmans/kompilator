#include <string>
#include <vector>

class Token
{
	std::vector<char> characters;
public:
	enum Type {
		WORD,
		WHITESPACE,
		ROUND_BRACKET_OPEN,
		ROUND_BRACKET_CLOSE,
		CURLY_BRACKET_OPEN,
		CURLY_BRACKET_CLOSE,
		SEMICOLON,
	};
	Type type;
	int line;
	int pos;
	Token(Token::Type type, int line, int pos);
	Token(Token::Type type, int line, int pos, char c);
	void addCharacter(char c);
	std::string getTypeName();
	std::string getContents();
};
