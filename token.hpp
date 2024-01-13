#ifndef LCC_TOKEN_H
#define LCC_TOKEN_H
#include <string> // string, string_view


class Token {
    std::string _lexeme;
    size_t row, column;
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

	explicit Token(const std::string lexeme, const Type type, size_t x, size_t y) : _lexeme(lexeme) {
        this->type = type;
        column = x;
        row = y;
    }

    std::string_view lexeme() const noexcept {
		return _lexeme;
	}

    size_t line() const {
        return row;
    }

    size_t cursor() const {
        return column;
    }
};
#endif