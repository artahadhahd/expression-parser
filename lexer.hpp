#ifndef LCC_LEXER_H
#define LCC_LEXER_H
#include <charconv> // from_chars
#include <cstdint>
#include <iostream> // getline, cout
#include <string> 	// string, string_view

#include "token.hpp"

class Lexer {
	uint64_t cursor = 0, line = 1;
	uint64_t buffer_size;
	std::string_view buffer;
public:
	explicit Lexer(std::string_view input);
    bool has() const noexcept;
    Token next() noexcept;
    void go_back() noexcept;
private:
    void skip_whitespace();
    [[nodiscard]] bool cursor_is_identifier_starter() const noexcept;
    [[nodiscard]] char cursor_is_identifier_body() const noexcept;
};
#endif