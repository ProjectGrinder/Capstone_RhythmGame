#include "../../include/game/utils/DSL/bullethell/tokenizer.h"

#include <stdexcept>

using namespace DSL;

Op_Type check_op(const std::string &src, const size_t i, __out int &length)
{
    length = 2;
    if(src.compare(i,length, "==")==0) return Op_Type::O_EQ;
    if(src.compare(i,length, "!=")==0) return Op_Type::O_NEQ;
    if(src.compare(i,length, "<=")==0) return Op_Type::O_LTE;
    if(src.compare(i,length, ">=")==0) return Op_Type::O_GTE;
    if(src.compare(i,length, "+=")==0) return Op_Type::O_ADD_ASSIGN;
    if(src.compare(i,length, "-=")==0) return Op_Type::O_SUB_ASSIGN;
    if(src.compare(i,length, "*=")==0) return Op_Type::O_MUL_ASSIGN;
    if(src.compare(i,length, "/=")==0) return Op_Type::O_DIV_ASSIGN;
    if(src.compare(i,length, "%=")==0) return Op_Type::O_MOD_ASSIGN;
    if(src.compare(i,length, "..")==0) return Op_Type::O_RANGE;
    if(src.compare(i,length, "&&")==0) return Op_Type::O_AND;
    if(src.compare(i,length, "||")==0) return Op_Type::O_OR;

    length = 1;
    if(src.compare(i,length, "+")==0) return Op_Type::O_ADD;
    if(src.compare(i,length, "-")==0) return Op_Type::O_SUB;
    if(src.compare(i,length, "*")==0) return Op_Type::O_MUL;
    if(src.compare(i,length, "/")==0) return Op_Type::O_DIV;
    if(src.compare(i,length, "%")==0) return Op_Type::O_MOD;
    if(src.compare(i,length, "=")==0) return Op_Type::O_ASSIGN;
    if(src.compare(i,length, ".")==0) return Op_Type::O_DOT;
    if(src.compare(i,length, "!")==0) return Op_Type::O_NOT;
    if(src.compare(i,length, "<")==0) return Op_Type::O_LT;
    if(src.compare(i,length, ">")==0) return Op_Type::O_GT;

    return Op_Type::O_UNKNOWN;
}

Symbol_Type check_symbol(const std::string &src, const size_t i, __out int &length)
{
    length = 1;
    if(src.compare(i,1, "(")==0) return Symbol_Type::S_LPAREN;
    if(src.compare(i,1, ")")==0) return Symbol_Type::S_RPAREN;
    if(src.compare(i,1, "{")==0) return Symbol_Type::S_LBRACE;
    if(src.compare(i,1, "}")==0) return Symbol_Type::S_RBRACE;
    if(src.compare(i,1, ",")==0) return Symbol_Type::S_COMMA;
    if(src.compare(i,1, "@")==0) return Symbol_Type::S_AMPERSAND;
    return Symbol_Type::S_UNKNOWN;
}

bool check_id_char(const char c)
{
    return isalnum(c) || c == '_';
}

Keyword_Type check_keyword(const std::string &src, const size_t i, __out int &length)
{
    auto checkNext = [&src,&i,&length]{ return i + length >= src.size() || !check_id_char(src[i + length]); };
    length = 6;
    if(src.compare(i,length, "ascent")==0 && checkNext()) return Keyword_Type::K_ASCENT;

    length = 5;
    if(src.compare(i,length, "false")==0 && checkNext()) return Keyword_Type::K_FALSE;

    length = 4;
    if(src.compare(i,length, "else")==0 && checkNext()) return Keyword_Type::K_ELSE;

    if(src.compare(i,length, "true")==0 && checkNext()) return Keyword_Type::K_TRUE;

    length = 2;
    if(src.compare(i,length, "if")==0 && checkNext()) return Keyword_Type::K_IF;


    return Keyword_Type::K_UNKNOWN;
}

std::vector<Token> DSL::tokenize(const std::string &src) {
    std::vector<Token> tokens;
    size_t i = 0;
    size_t line = 1;

    while (i < src.size()) {
        // skip whitespace
        while (i < src.size() && (src[i] == ' ' || src[i] == '\t' || src[i] == '\r'))
            i++;

        if (i >= src.size())
            break;

        const char c = src[i];

        // newline
        if (c == '\n') {
            tokens.push_back({Token_Type::T_NEWLINE, "\\n"});
            i++;
            line++;
            continue;
        }

        // comment
        if (c == '/' && i + 1 < src.size() && src[i + 1] == '/') {
            i += 2;
            while (i < src.size() && src[i] != '\n') i++;
            continue;
        }

        if (c == '/' && i + 1 < src.size() && src[i + 1] == '*') {
            i += 2;
            bool done = false;
            while (i+1 < src.size())
            {
                if (src[i] == '\n') line++;
                else if (src[i] == '*' && src[i + 1] == '/')
                {
                    done = true;
                    break;
                }
                i++;
            }
            if (!done) throw std::runtime_error("DSL line " + std::to_string(line) +  " error : Comment /* with no ending?");
            i+=2;
            continue;
        }

        // ignore ; for C lang lover
        if (c == ';') {
            i++;
            continue;
        }

        // op
        int length = 0;
        if (Op_Type op_type; (op_type = check_op(src, i, length)) != Op_Type::O_UNKNOWN)
        {
            Token token;
            token.type = Token_Type::T_OP;
            token.text = src.substr(i, length);
            token.op_type = op_type;
            tokens.push_back(token);
            i+=length;
            continue;
        }

        if (Symbol_Type symbol_type; (symbol_type = check_symbol(src,i, length)) != Symbol_Type::S_UNKNOWN)
        {
            Token token;
            token.type = Token_Type::T_SYMBOL;
            token.text = src.substr(i, 1);
            token.symbol_type = symbol_type;
            tokens.push_back(token);
            i+=length;
            continue;
        }

        if (Keyword_Type keyword_type; (keyword_type = check_keyword(src,i, length)) != Keyword_Type::K_UNKNOWN)
        {
            Token token;
            token.type = Token_Type::T_KEYWORD;
            token.text = src.substr(i, length);
            token.keyword_type = keyword_type;
            tokens.push_back(token);
            i+=length;
            continue;
        }

        // number
        if (isdigit(c)) {
            const size_t start = i++;
            bool floating_point = false;

            while (i < src.size())
            {
                if (isdigit(src[i]))
                {
                    i++;
                    continue;
                }
                if(src[i] == '.')
                {
                    if(i + 1 < src.size() && src[i + 1] == '.')
                        break;

                    if(floating_point)
                        throw std::runtime_error("DSL line " + std::to_string(line) +  " error : 2 floating points?");

                    floating_point = true;
                    i++;
                    continue;
                }

                break;
            }


            tokens.push_back({
                Token_Type::T_NUM,
                src.substr(start, i - start)
            });
            continue;
        }

        // --- identifier ---
        if (isalpha(c) || c == '_') {
            const size_t start = i;

            while (i < src.size() && (isalnum(src[i]) || src[i] == '_'))
                i++;

            tokens.push_back({
                Token_Type::T_ID,
                src.substr(start, i - start)
            });
            continue;
        }

        // --- Unknown character ---
        throw std::runtime_error("DSL line" + std::to_string(line) +  " error : What do you mean by \"" + c + "\"?");
    }

    tokens.push_back({Token_Type::T_END, ""});
    return tokens;
}