#include "game/utils/Bullethell_DSL/parser.h"

#include <stdexcept>

#include "utils/print_debug.h"

using namespace DSL;

int get_order(const Op_Type op)
{
    switch(op)
    {
    case Op_Type::O_MUL:
    case Op_Type::O_DIV:
    case Op_Type::O_MOD:
        return 20;

    case Op_Type::O_ADD:
    case Op_Type::O_SUB:
        return 10;

    case Op_Type::O_EQ:
    case Op_Type::O_NEQ:
    case Op_Type::O_LT:
    case Op_Type::O_GT:
    case Op_Type::O_LTE:
    case Op_Type::O_GTE:
        return 5;

    case Op_Type::O_AND:
        return 3;

    case Op_Type::O_OR:
        return 2;

    default:
        return -1;
    }
}

void validate_eof(const std::vector<Token> &tokens, size_t &pos, size_t &line)
{
    if (pos >= tokens.size() || tokens[pos].type == Token_Type::T_END)
    {
        throw std::runtime_error("DSL line " + std::to_string(line) + " error : Unexpected End Of File");
    }
}

void skip_newline(const std::vector<Token> &tokens, size_t &pos, size_t &line)
{
    while (tokens[pos].type == Token_Type::T_NEWLINE)
    {
        pos++;
        line++;
    }
    validate_eof(tokens, pos, line);
}

std::unique_ptr<Expr> DSL::extract_binary_expr(const std::vector<Token>& tokens, size_t& pos, size_t& line, int min_order)
{
    validate_eof(tokens, pos, line);
    auto left = extract_primary_expr(tokens, pos, line);
    while (true)
    {
        if (tokens[pos].type != Token_Type::T_OP)
            break;

        const Op_Type op = tokens[pos].op_type;
        const int order = get_order(op);

        if (order < min_order)
            break;

        pos++;
        auto right = extract_binary_expr(tokens,pos,line,order + 1);
        left = std::make_unique<Expr>(BinaryExpr{ op, std::move(left), std::move(right) });
    }

    return left;
}

std::unique_ptr<Expr> DSL::extract_function_expr(const std::vector<Token>& tokens, size_t& pos, size_t& line)
{
    validate_eof(tokens, pos, line);
    FunctionExpr func;
    func.name = tokens[pos].text;

    pos += 2;

    while (true)
    {
        validate_eof(tokens, pos, line);

        if (tokens[pos].type == Token_Type::T_SYMBOL && tokens[pos].symbol_type == Symbol_Type::S_RPAREN)
        {
            pos++;
            break;
        }

        func.args.push_back(
            extract_binary_expr(tokens, pos, line, 0));
        validate_eof(tokens, pos, line);

        if (tokens[pos].type == Token_Type::T_SYMBOL && tokens[pos].symbol_type == Symbol_Type::S_COMMA)
        {
            pos++;
            continue;
        }

        if (tokens[pos].type == Token_Type::T_SYMBOL && tokens[pos].symbol_type == Symbol_Type::S_RPAREN)
        {
            pos++;
            break;
        }

        throw std::runtime_error(
            "DSL line " + std::to_string(line) + " error : Expected , or )");
    }

    return std::make_unique<Expr>(std::move(func));
}

std::unique_ptr<Expr> DSL::extract_primary_expr(const std::vector<Token>& tokens, size_t& pos, size_t& line)
{
    validate_eof(tokens, pos, line);
    const Token& token = tokens[pos];

    // number
    if (token.type == Token_Type::T_NUM)
    {
        pos++;
        return std::make_unique<Expr>(NumberExpr{std::stof(token.text)});
    }

    // boolean
    if (token.type == Token_Type::T_KEYWORD)
    {
        if (token.keyword_type == Keyword_Type::K_TRUE)
        {
            pos++;
            return std::make_unique<Expr>(BooleanExpr{true});
        }

        if (token.keyword_type == Keyword_Type::K_FALSE)
        {
            pos++;
            return std::make_unique<Expr>(BooleanExpr{false});
        }
    }

    // identifier / function
    if (token.type == Token_Type::T_ID)
    {
        // function
        if (pos+1 < tokens.size() && tokens[pos + 1].type == Token_Type::T_SYMBOL && tokens[pos + 1].symbol_type == Symbol_Type::S_LPAREN)
            return extract_function_expr(tokens, pos, line);

        // variable
        pos++;
        return std::make_unique<Expr>(VariableExpr{token.text});
    }

    // unary
    if (token.type == Token_Type::T_OP)
    {
        if (token.op_type == Op_Type::O_SUB || token.op_type == Op_Type::O_ADD || token.op_type == Op_Type::O_NOT)
        {
            const Op_Type op = token.op_type;
            pos++;
            auto value = extract_primary_expr(tokens, pos, line);
            return std::make_unique<Expr>( UnaryExpr{ op, std::move(value) });
        }
    }

    // grouped expr
    if (token.type == Token_Type::T_SYMBOL &&
        token.symbol_type == Symbol_Type::S_LPAREN)
    {
        pos++;

        auto expr =
            extract_binary_expr(tokens, pos, line, 0);

        if (tokens[pos].type != Token_Type::T_SYMBOL || tokens[pos].symbol_type != Symbol_Type::S_RPAREN)
            throw std::runtime_error("DSL line " + std::to_string(line) + " error : Expected )");

        pos++;
        return expr;
    }

    throw std::runtime_error("DSL line " + std::to_string(line) + " error : Invalid expression");
}


std::unique_ptr<Statement> DSL::parse_block(const std::vector<Token> &tokens, size_t &pos, size_t &line)
{
    BlockStatement block;
    const size_t start_line = line;
    skip_newline(tokens, pos, line);
    if (tokens[pos].type != Token_Type::T_SYMBOL ||tokens[pos].symbol_type != Symbol_Type::S_LBRACE)
        throw std::runtime_error("DSL line " + std::to_string(line) + " error : Expected { for initing a block");
    pos++;
    while (true)
    {
        validate_eof(tokens, pos, line);
        if (tokens[pos].type == Token_Type::T_END)
            throw std::runtime_error("DSL line " + std::to_string(line) +  " error : Expect }");

        skip_newline(tokens, pos, line);

        if (tokens[pos].type == Token_Type::T_SYMBOL && tokens[pos].symbol_type == Symbol_Type::S_RBRACE)
        {
            pos++;
            break;
        }

        block.body.push_back(parse_line(tokens, pos, line));
    }
    return std::make_unique<Statement>(std::move(block), start_line);
}
std::unique_ptr<Statement> DSL::parse_if(const std::vector<Token> &tokens, size_t &pos, size_t &line)
{
    IfStatement stmt;
    const size_t start_line = line;
    validate_eof(tokens, pos, line);
    while (true)
    {
        IfBranch branch;
        bool has_condition = true;

        pos++;

        if (tokens[pos-1].type == Token_Type::T_KEYWORD && tokens[pos-1].keyword_type == Keyword_Type::K_ELSE)
        {
            if (tokens[pos].type == Token_Type::T_KEYWORD && tokens[pos].keyword_type == Keyword_Type::K_IF)
                pos++;
            else
                has_condition = false;
        }
        skip_newline(tokens, pos, line);
        if (has_condition)
        {
            if (tokens[pos].type != Token_Type::T_SYMBOL || tokens[pos].symbol_type != Symbol_Type::S_LPAREN)
                throw std::runtime_error("DSL line " + std::to_string(line) + " error : Expected ( after if");

            pos++;

            branch.condition = extract_binary_expr(tokens, pos, line);
            validate_eof(tokens, pos, line);
            if (tokens[pos].type != Token_Type::T_SYMBOL || tokens[pos].symbol_type != Symbol_Type::S_RPAREN)
                throw std::runtime_error("DSL line " + std::to_string(line) + " error : Expected ) after if");

            pos++;
        }
        auto body = parse_block(tokens, pos, line);

        auto* block = std::get_if<BlockStatement>(&body->stmt);

        if (has_condition)
        {
            branch.body = std::move(block->body);
            stmt.branches.push_back(std::move(branch));
        }
        else
        {
            stmt.else_body = std::move(block->body);
            break;
        }

        size_t temp_pos = pos;
        size_t temp_line = line;

        skip_newline(tokens, pos, line);
        if (!(tokens[pos].type == Token_Type::T_KEYWORD && tokens[pos].keyword_type == Keyword_Type::K_ELSE))
        {
            pos = temp_pos;
            line = temp_line;
            break;
        }
    }
    return std::make_unique<Statement>(std::move(stmt), start_line);
}
std::unique_ptr<Statement> DSL::parse_ascent(const std::vector<Token> &tokens, size_t &pos, size_t &line)
{
    AscentStatement stmt;
    const size_t start_line = line;
    validate_eof(tokens, pos, line);
    if (tokens[pos].type != Token_Type::T_SYMBOL ||tokens[pos].symbol_type != Symbol_Type::S_LPAREN)
        throw std::runtime_error("DSL line " + std::to_string(line) + " error : Expected ( after ascent");

    pos++;
    validate_eof(tokens, pos, line);
    if (tokens[pos].type != Token_Type::T_ID)
        throw std::runtime_error("DSL line " + std::to_string(line) + " error : Expected iterator name to ascent");

    stmt.iterator = tokens[pos].text;
    pos++;
    validate_eof(tokens, pos, line);
    if (tokens[pos].text != "in")
        throw std::runtime_error("DSL line " + std::to_string(line) + " error : Expected \"in\" in ascent");

    pos++;
    validate_eof(tokens, pos, line);
    stmt.start = extract_binary_expr(tokens, pos, line);
    validate_eof(tokens, pos, line);
    if (tokens[pos].type != Token_Type::T_OP || tokens[pos].op_type != Op_Type::O_RANGE)
        throw std::runtime_error("DSL line " + std::to_string(line) + " error : Expected .. in ascent");

    pos++;
    validate_eof(tokens, pos, line);
    stmt.end = extract_binary_expr(tokens, pos, line);
    validate_eof(tokens, pos, line);
    if (tokens[pos].type != Token_Type::T_SYMBOL ||tokens[pos].symbol_type != Symbol_Type::S_RPAREN)
        throw std::runtime_error("DSL line " + std::to_string(line) + " error : Expected ) after ascent");

    pos++;

    auto body = parse_block(tokens, pos, line);

    auto* block = std::get_if<BlockStatement>(&body->stmt);

    stmt.body = std::move(block->body);

    return std::make_unique<Statement>(std::move(stmt), start_line);
}

std::unique_ptr<Statement> DSL::parse_spawn(const std::vector<Token> &tokens, size_t &pos, size_t &line)
{
    SpawnStatement stmt;
    const size_t start_line = line;
    validate_eof(tokens, pos, line);
    if (tokens[pos].type != Token_Type::T_SYMBOL ||tokens[pos].symbol_type != Symbol_Type::S_LPAREN)
        throw std::runtime_error("DSL line " + std::to_string(line) + " error : Expected ( after spawn");
    pos++;

    while (true)
    {
        validate_eof(tokens, pos, line);

        stmt.args.push_back(
            extract_binary_expr(tokens, pos, line));

        if (tokens[pos].type == Token_Type::T_SYMBOL)
        {
            if (tokens[pos].symbol_type == Symbol_Type::S_COMMA)
            {
                pos++;
                continue;
            }

            if (tokens[pos].symbol_type == Symbol_Type::S_RPAREN)
            {
                pos++;
                break;
            }
        }

        throw std::runtime_error("DSL line " + std::to_string(line) + " error : Expected , or )");
    }

    return std::make_unique<Statement>(std::move(stmt), start_line);
}
std::unique_ptr<Statement> DSL::parse_assign(const std::vector<Token> &tokens, size_t &pos,size_t &line)
{
    const std::string name = tokens[pos].text;
    pos++;
    validate_eof(tokens, pos, line);
    const Token &op = tokens[pos];
    if (op.type != Token_Type::T_OP)
        throw std::runtime_error("DSL line " + std::to_string(line) +  " error : I don't understand the statement. " + name + "and then " + op.text + "?");
    if (!(op.op_type >= Op_Type::O_ASSIGN && op.op_type <= Op_Type::O_MOD_ASSIGN))
        throw std::runtime_error("DSL line " + std::to_string(line) +  " error : I don't understand the statement. What do you mean \"" + op.text + "\" as an assign statement?");
    pos++;
    return std::make_unique<Statement>(AssignStatement{name, op.op_type, extract_binary_expr(tokens, pos, line,0)}, line);
}

std::unique_ptr<Statement> DSL::parse_bpm(const std::vector<Token> &tokens, size_t &pos,size_t &line)
{
    const std::string name = "@" + tokens[pos].text;
    pos++;
    validate_eof(tokens, pos, line);
    const Token &op = tokens[pos];
    if (op.type != Token_Type::T_OP && op.op_type != Op_Type::O_ASSIGN)
        throw std::runtime_error("DSL line " + std::to_string(line) +  " error : To config the system, you use @var = x");
    pos++;
    return std::make_unique<Statement>(AssignStatement{name, op.op_type, extract_binary_expr(tokens, pos, line,0)}, line);
}

std::unique_ptr<Statement> DSL::parse_line(const std::vector<Token> &tokens, size_t &pos, size_t &line) {
    const Token& cmd = tokens[pos];

    if (cmd.type == Token_Type::T_KEYWORD)
    {
        if (cmd.keyword_type == Keyword_Type::K_IF)
        {
            return parse_if(tokens, pos, line);
        }

        if (cmd.keyword_type == Keyword_Type::K_ASCENT)
        {
            pos++;
            return parse_ascent(tokens, pos, line);
        }
    }

    if (cmd.type == Token_Type::T_SYMBOL)
    {
        if (cmd.symbol_type == Symbol_Type::S_LBRACE)
            return parse_block(tokens, pos, line);
        if (cmd.symbol_type == Symbol_Type::S_AMPERSAND)
        {
            pos++;
            return parse_bpm(tokens, pos, line);
        }

    }

    if (cmd.type == Token_Type::T_ID)
    {
        if (cmd.text == "spawn")
        {
            pos++;
            return parse_spawn(tokens, pos, line);
        }
        return parse_assign(tokens, pos, line);
    }

    throw std::runtime_error("DSL line " + std::to_string(line) +  " error : I don't understand the statement");
}

AST DSL::parser(const std::vector<Token> &tokens)
{
    AST parsed;
    auto &statements = parsed.statements;
    size_t pos = 0;
    size_t line = 1;

    while (tokens[pos].type != Token_Type::T_END) {
        while (tokens[pos].type == Token_Type::T_NEWLINE) {
            pos++;
            line++;
        }

        if (tokens[pos].type == Token_Type::T_END) break;

        statements.push_back(parse_line(tokens, pos, line));

        if (tokens[pos].type != Token_Type::T_NEWLINE &&
            tokens[pos].type != Token_Type::T_END &&
            !(tokens[pos].type == Token_Type::T_SYMBOL && tokens[pos].symbol_type == Symbol_Type::S_RBRACE))
        {
            throw std::runtime_error("DSL line " + std::to_string(line) + " error : Expected newline after statement");
        }
    }

    return parsed;
}