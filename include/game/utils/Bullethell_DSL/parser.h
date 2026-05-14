#pragma once
#include <memory>
#include <variant>

#include "tokenizer.h"

namespace DSL
{
    struct Expr;

    struct NumberExpr
    {
        float value;
    };

    struct VariableExpr
    {
        std::string name;
    };

    struct BooleanExpr
    {
        bool value;
    };
    struct UnaryExpr
    {
        Op_Type op;
        std::unique_ptr<Expr> value;
    };

    struct BinaryExpr
    {
        Op_Type op;
        std::unique_ptr<Expr> left;
        std::unique_ptr<Expr> right;
    };

    struct FunctionExpr
    {
        std::string name;
        std::vector<std::unique_ptr<Expr>> args;
    };

    using ExprVariant = std::variant<NumberExpr, VariableExpr, BooleanExpr, UnaryExpr, BinaryExpr, FunctionExpr>;

    struct Expr
    {
        ExprVariant expr;
        template<typename T>
        explicit Expr(T v) : expr(std::move(v)) {}
    };

    struct Statement;

    struct SpawnStatement
    {
        std::vector<std::unique_ptr<Expr>> args;
    };

    struct AssignStatement
    {
        std::string name;
        Op_Type op;
        std::unique_ptr<Expr> value;
    };

    struct AscentStatement
    {
        std::string iterator;
        std::unique_ptr<Expr> start;
        std::unique_ptr<Expr> end;
        std::vector<std::unique_ptr<Statement>> body;
    };

    struct BlockStatement
    {
        std::vector<std::unique_ptr<Statement>> body;
    };

    struct IfBranch
    {
        std::unique_ptr<Expr> condition;
        std::vector<std::unique_ptr<Statement>> body;
    };

    struct IfStatement
    {
        std::vector<IfBranch> branches;
        std::vector<std::unique_ptr<Statement>> else_body;
    };

    using StmtVariant = std::variant<
        SpawnStatement,
        AssignStatement,
        AscentStatement,
        BlockStatement,
        IfStatement
    >;

    struct Statement
    {
        size_t line;
        StmtVariant stmt;
        template<typename T>
        explicit Statement(T v, size_t line) : stmt(std::move(v)), line(line) {}
    };

    struct AST
    {
        std::vector<std::unique_ptr<Statement>> statements;
    };

    std::unique_ptr<Expr> extract_binary_expr(const std::vector<Token>& tokens, size_t& pos, const size_t& line, const int min_order = 0);
    std::unique_ptr<Expr> extract_function_expr(const std::vector<Token>& tokens, size_t& pos, const size_t& line);

    std::unique_ptr<Expr> extract_primary_expr(const std::vector<Token>& tokens, size_t& pos, const size_t& line);

    std::unique_ptr<Statement> parse_block(const std::vector<Token> &tokens, size_t &pos, size_t &line);
    std::unique_ptr<Statement> parse_if(const std::vector<Token> &tokens, size_t &pos, size_t &line);
    std::unique_ptr<Statement> parse_ascent(const std::vector<Token> &tokens, size_t &pos, size_t &line);

    std::unique_ptr<Statement> parse_spawn(const std::vector<Token> &tokens, size_t &pos, size_t &line);
    std::unique_ptr<Statement> parse_assign(const std::vector<Token> &tokens, size_t &pos,const size_t &line);

    std::unique_ptr<Statement> parse_line(const std::vector<Token> &tokens, size_t &pos, size_t &line);

    AST parser(const std::vector<Token> &tokens);
}

