#include "lexer.hpp"

/* Public methods */

Lexer::Lexer(std::string_view input) : buffer(input)
{
    buffer_size = input.size();
}

bool Lexer::has() const noexcept
{
    return cursor < buffer_size;
}

Token Lexer::next() noexcept
{
    skip_whitespace();
    if (!has()) {
        return Token("TOKEN_END", Token::Type::End, cursor, line);
    }

    std::string lexeme;
    char c = buffer[cursor];

    if (c >= '1' && c <= '9') {
        while (has() && isdigit(c)) {
            lexeme += c;
            ++cursor;
            c = buffer[cursor];
        }
        return Token(lexeme, Token::Type::Num, cursor, line);
    }
    if (c == '0') {
        ++cursor;
        lexeme += c;
        return Token(lexeme, Token::Type::Num, cursor, line);
    }
    if (cursor_is_identifier_starter()) {
        while ((c = cursor_is_identifier_body()) != 0) {
            lexeme += c;
            ++cursor;
        }
        return Token(lexeme, Token::Type::Ident, cursor, line);
    }

    c = buffer[cursor];
    switch (c) {
    case '(':
        ++cursor;
        return Token("(", Token::Type::LParen, cursor, line);
    case ')':
        ++cursor;
        return Token(")", Token::Type::RParen, cursor, line);
    case '+':
        ++cursor;
        return Token("+", Token::Type::Add, cursor, line);
    case '-':
        ++cursor;
        return Token("-", Token::Type::Sub, cursor, line);
    case '*':
        ++cursor;
        return Token("*", Token::Type::Mul, cursor, line);
    case '/':
        ++cursor;
        return Token("/", Token::Type::Div, cursor, line);
    }

    return Token("<unregistered character>", Token::Type::Unexpected, cursor, line);
}

void Lexer::go_back() noexcept {
    if (cursor > 0)
        --cursor;
}

/* Private methods */

void Lexer::skip_whitespace()
{
    bool go = true;
    while (has() && go) {
        switch (buffer[cursor]) {
        case ' ':
        case '\t':
        case '\r':
        case '\v':
            ++cursor;
            break;
        case '\n':
            ++line;
            ++cursor;
            break;
        case '#':
            do {
                ++cursor;
            } while (has() && buffer[cursor] != '\n');
            break;
        default:
            go = false;
            break;
        }
    }
}

[[nodiscard]] bool Lexer::cursor_is_identifier_starter() const noexcept
{
    if (!has()) {
        return 0;
    }
    return isalpha(buffer[cursor]);
}

[[nodiscard]] char Lexer::cursor_is_identifier_body() const noexcept
{
    if (!has()) {
        return 0;
    }
    const char c = buffer[cursor];
    if (isalnum(c) || c == '_') {
        return c;
    }
    return 0;
}