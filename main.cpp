#include <iostream>
#include <iomanip>
#include <optional>

#include "tree.hpp"

// struct TreeNode {
//     Token data;
//     TreeNode* left;
//     TreeNode* right;
// };

#include "lexer.hpp"
using ErrorType = std::string;

struct  {
    ErrorType err;
    bool has_failed;
} compiler_state = {
    "", false
};

std::optional<int64_t> low_prec(Lexer& lexer);

std::optional<int64_t> check_for_sign(Lexer& lexer, Token& after) {
    int64_t sign = 1;
    while (1)
    {
        if (after.type == Token::Type::Add)
            ;
        else if (after.type == Token::Type::Sub) {
            sign = (sign == 1) ? -1 : 1;
        } else {
            if (after.type == Token::Type::Num) {
                return sign;
            }
            return std::nullopt;
        }
        after = lexer.next();
    }
#if defined(_MSC_VER) && !defined(__clang__)
    __assume(false);
#else
    __builtin_unreachable();
#endif
}

[[nodiscard]] std::optional<int64_t> parse_factor(Lexer& lexer) {
    Token next = lexer.next();
    int64_t sign = 1;
    if (next.type == Token::Type::Add || next.type == Token::Type::Sub) {
        auto is_signed = check_for_sign(lexer, next);
        if (!is_signed) {
            compiler_state.err = next.lexeme;
            compiler_state.err +=  ": not a valid unary operator";
            return std::nullopt;
        }
        sign = is_signed.value();
    }
    if (next.type != Token::Type::Num && next.type != Token::Type::LParen) {
        compiler_state.err = "Expected a number, got TOKEN#";
        compiler_state.err += std::to_string((int)next.type);
        compiler_state.has_failed = true;
		return std::nullopt;
	}
    int64_t result;
    if (next.type == Token::Type::Num) {
        std::string_view number = next.lexeme;
        auto [_, errc] = std::from_chars(number.data(), number.data() + number.size(), result);
        if (errc == std::errc::result_out_of_range) {
            compiler_state.err = "Number '";
            compiler_state.err += number;
            compiler_state.err += "' is too big to compute";
            return std::nullopt;
        }
    } else if (next.type == Token::Type::LParen) {
        auto factor = low_prec(lexer);
        if (!factor) {
            std::cout << "my nuts";
            return std::nullopt;
        }
        result = factor.value();
        lexer.go_back();
        next = lexer.next();
        if (next.type != Token::Type::RParen) {
            compiler_state.err = "Expected a ), got ";
            compiler_state.err += next.lexeme;
            compiler_state.has_failed = true;
            return std::nullopt;
        }
    }
	return sign * result;
}

[[nodiscard]] std::optional<int64_t> higher_prec(Lexer& lexer) {
    auto left = parse_factor(lexer);
    if (!left) {
        return std::nullopt;
    }
    int64_t num = left.value();
    auto next = lexer.next();
    while (next.type == Token::Type::Mul || next.type == Token::Type::LParen || next.type == Token::Type::Div || next.type == Token::Type::Num)
    {
        std::optional<int64_t> right;
        if (next.type == Token::Type::Num) {
            right = std::stoll(next.lexeme);
        }
        else if (next.type == Token::Type::Mul || next.type == Token::Type::Div) {
            right = parse_factor(lexer);
        } else {
            right = low_prec(lexer);
        }
        if (!right) {
            return std::nullopt;
        }
        if (next.type == Token::Type::Div) {
            num /= right.value();
        } else {
            num *= right.value();
        }
        next = lexer.next();
    }
    lexer.go_back();
    return num;
}

[[nodiscard]] std::optional<int64_t> low_prec(Lexer& lexer) {
    auto left = higher_prec(lexer);
    if (!left) {
        return std::nullopt;
    }
    int64_t num = left.value();
    Token next = lexer.next();
    while (next.type == Token::Type::Add || next.type == Token::Type::Sub)
    {
        auto right = higher_prec(lexer);
        if (!right) {
            return std::nullopt;
        }
        if (next.type == Token::Type::Add) {
            num += right.value();
        } else if (next.type == Token::Type::Sub) {
            num -= right.value();
        } else {
            break;
        }
        next = lexer.next();
    }
    return num;
}

int main()
{
    while (true) {
        std::string input;
        std::cout << "lcc> ";
        std::getline(std::cin, input);

        if (input == "") {
            continue;
        }
        if (input == ":q" || input == "exit") {
            break;
        }

        {
            Lexer lexer(input);
            auto parsed = low_prec(lexer);
            if (!parsed) {
                std::cerr << "Error: " << compiler_state.err;
            } else {
                std::cout << parsed.value();
            }
            std::cout << '\n';
        }
    }
    return EXIT_SUCCESS;
}