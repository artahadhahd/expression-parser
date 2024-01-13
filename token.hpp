#ifndef LCC_TOKEN_H
#define LCC_TOKEN_H
#include <string> // string, string_view


struct Token {
    std::string lexeme;
    size_t line, cursor;
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

	explicit Token(const std::string lexeme_, const Type type, size_t x, size_t y) : lexeme(lexeme_) {
        this->type = type;
        cursor = x;
        line = y;
    }
};
#endif