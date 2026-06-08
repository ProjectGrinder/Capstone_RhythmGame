#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace DSL
{
    enum class Token_Type : uint8_t {
        T_END,
        T_ID,
        T_NUM,
        T_OP,
        T_SYMBOL,
        T_KEYWORD,
        T_NEWLINE,
        T_COMMENT,
        T_UNKNOWN
    };

    enum class Op_Type : uint8_t {
        O_ASSIGN,
        O_ADD_ASSIGN,
        O_SUB_ASSIGN,
        O_MUL_ASSIGN,
        O_DIV_ASSIGN,
        O_MOD_ASSIGN,

        O_ADD,
        O_SUB,
        O_MUL,
        O_DIV,
        O_MOD,

        O_EQ,
        O_NEQ,
        O_LT,
        O_GT,
        O_LTE,
        O_GTE,
        O_RANGE,
        O_DOT,
        O_AND,
        O_OR,
        O_NOT,
        O_UNKNOWN
    };

    enum class Symbol_Type : uint8_t
    {
        S_LPAREN,
        S_RPAREN,
        S_LBRACE,
        S_RBRACE,
        S_COMMA,
        S_AMPERSAND,
        S_UNKNOWN
    };

    enum class Keyword_Type : uint8_t
    {
        K_IF,
        K_ELSE,
        K_ASCENT,
        K_TRUE,
        K_FALSE,
        K_UNKNOWN
    };

    struct Token
    {
        Token_Type type;
        std::string text;

        Op_Type op_type = Op_Type::O_UNKNOWN;
        Symbol_Type symbol_type = Symbol_Type::S_UNKNOWN;
        Keyword_Type keyword_type = Keyword_Type::K_UNKNOWN;
    };

    std::vector<Token> tokenize(const std::string &src);
}
