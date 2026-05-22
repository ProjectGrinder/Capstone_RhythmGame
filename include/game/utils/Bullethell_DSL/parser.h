#pragma once
#include <stdint.h>
#include "tokenizer.h"

enum class Op {
    SET,
    ADD,
    SELECT,
    FIRE,
    SLEEP,
    LOOP_START,
    LOOP_END,
    INIT,
    PUSH,
    CLEAR,
    IF,
    ELSE,
    END_IF
};

struct Instruction {
    Op op;
    std::vector<std::string> args;
    int jump = -1;
};

struct Parser
{
    std::vector<Token> &tokens;
    size_t pos = 0;
    size_t line = 1;

    Token &current()
    {
        if (pos >= tokens.size())
            return tokens.back();
        return tokens[pos];
    }
    Token &advance()
    {
        return tokens[pos++];
    }

    std::string expect_ident(bool optional = false) {
        Token &t = current();
        if (t.type != Token_Type::T_IDENT)
        {
            if (optional) return "";
            throw std::runtime_error("DSL line " + std::to_string(line) +  " error : Expected identifier");
        }
        return advance().text;
    }

    std::string expect_expression(bool optional = false) {
        Token &t = current();

        if (t.type == Token_Type::T_EXP)
            return advance().text;
        if (optional) return "";

        throw std::runtime_error("DSL line " + std::to_string(line) +  " error : Expected expression");
    }

    std::string expect_value(bool optional = false) {
        Token &t = current();

        if (t.type == Token_Type::T_NUM ||
            t.type == Token_Type::T_IDENT ||
            t.type == Token_Type::T_EXP)
            return advance().text;
        if (optional) return "";

        throw std::runtime_error("DSL line " + std::to_string(line) +  " error : Expected value");
    }

    Instruction parse_line() {
        Token cmd = advance();

        if (cmd.type != Token_Type::T_IDENT)
            throw std::runtime_error("DSL line " + std::to_string(line) +  " error : Shouldn't you start with Operation?");

        if (cmd.text == "SET") return {Op::SET, {expect_ident(), expect_value()}};
        if (cmd.text == "ADD") return {Op::ADD, {expect_ident(), expect_value()}};
        if (cmd.text == "SELECT") return {Op::SELECT, {expect_ident(), expect_value(true), expect_value(true)}};
        if (cmd.text == "LOOP_START") return {Op::LOOP_START, {}};
        if (cmd.text == "LOOP_END") return {Op::LOOP_END, {expect_ident(), expect_value()}};
        if (cmd.text == "SLEEP") return {Op::SLEEP, {expect_value()}};
        if (cmd.text == "FIRE") return {Op::FIRE, {expect_value(true), expect_value(true)}};
        if (cmd.text == "CLEAR") return {Op::CLEAR, {expect_ident(), expect_value(true), expect_value(true)}};
        if (cmd.text == "IF") return {Op::IF, {expect_expression()}};
        if (cmd.text == "ELSE") return {Op::ELSE, {}};
        if (cmd.text == "END_IF") return {Op::END_IF, {}};
        if (cmd.text == "INIT") return {Op::INIT, {expect_ident(), expect_value(true), expect_value(true)}};
        if (cmd.text == "PUSH") return {Op::PUSH, {expect_ident()}};



        throw std::runtime_error("DSL line " + std::to_string(line) +  " error : What is that operation?");
    }

    std::vector<Instruction> parse() {
        std::vector<Instruction> program;

        while (current().type != Token_Type::T_END) {
            while (current().type == Token_Type::T_NEWLINE) {
                advance();
                line++;
            }

            if (current().type == Token_Type::T_END) break;

            program.push_back(parse_line());
        }

        return program;
    }

};

