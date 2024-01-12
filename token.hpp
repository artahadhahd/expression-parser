#ifndef LCC_TOKEN_H
#define LCC_TOKEN_H
#include <string> // string, string_view

class Token {
    std::string _lexeme;
public:
	enum class Type : uint8_t {
		Unexpected,
		End,
		Num,
		Add,
		Sub,
		Mul,
		Div,
		LParen,
		RParen,
		Ident,
	} type;

	explicit Token(const std::string lexeme, const Type type) : _lexeme(lexeme) {
        this->type = type;
    }

    std::string_view lexeme() const noexcept {
		return _lexeme;
	}
};
#endif