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

Token Lexer::next()
{
    skip_whitespace();
    if (!has()) {
        return Token("TOKEN_END", Token::Type::End, cursor, line);
    }

    std::string lexeme;
    char c;

    if (cursor_is_num() != 0) {
        while ((c = cursor_is_num()) != 0) {
            lexeme += c;
            ++cursor;
        }
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
    }

    return Token("<unregistered character>", Token::Type::Unexpected, cursor, line);
}

void Lexer::go_back() {
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

[[nodiscard]] char Lexer::cursor_is_num() const
{
    if (!has()) {
        return 0;
    }
    const char c = buffer[cursor];
    if (c <= '9' && c >= '1') {
        return c;
    }
    return 0;
}

[[nodiscard]] char Lexer::cursor_is_operator() const
{
    if (!has()) {
        return 0;
    }
    char c = 0;
    switch ((c = buffer.at(cursor))) {
    case '+':
    case '/':
    case '*':
    case '-':
        return c;
    }
    return 0;
}

[[nodiscard]] bool Lexer::cursor_is_identifier_starter() const
{
    if (!has()) {
        return 0;
    }
    return isalpha(buffer[cursor]);
}

[[nodiscard]] char Lexer::cursor_is_identifier_body() const
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