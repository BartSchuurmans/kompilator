#include <string>
#include <vector>

#define KOMPILATOR_TOKEN_TYPES(def)	\
	def(WHITESPACE)	\
	def(WORD)	\
	def(NUMBER)	\
	def(SEMICOLON)	\
	def(COLON)	\
	def(COMMA)	\
	def(FORWARD_SLASH)	\
	def(PLUS)	\
	def(MINUS)	\
	def(EQUALS)	\
	def(LESS_THAN)	\
	def(LESS_THAN_EQUALS)	\
	def(GREATER_THAN)	\
	def(GREATER_THAN_EQUALS)	\
	def(ROUND_BRACKET_OPEN)	\
	def(ROUND_BRACKET_CLOSE)	\
	def(CURLY_BRACKET_OPEN)	\
	def(CURLY_BRACKET_CLOSE)	\
	def(SQUARE_BRACKET_OPEN)	\
	def(SQUARE_BRACKET_CLOSE)

enum class token_type {
	#define DEFINE_ENUM_TYPE(v) v,
	KOMPILATOR_TOKEN_TYPES(DEFINE_ENUM_TYPE)
	#undef DEFINE_ENUM_TYPE
};

class token
{
public:
	token_type type;
	std::vector<char> characters;
	int line;
	int pos;

	token(int line, int pos);
	token(int line, int pos, char c);

	void add_character(char c);
	std::string get_type_name();
	std::string get_contents();
};
