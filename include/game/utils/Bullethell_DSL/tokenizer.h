#pragma once
#include <stdint.h>

extern "C"{
    typedef unsigned long       DWORD;
    DWORD file_read(_Out_ FileContent **content, const char *name);
    void file_free(FileContent **file);
}

enum class Token_Type : uint8_t {
    T_END,
    T_IDENT,
    T_NUM,
    T_EXP,
    T_NEWLINE,
    T_COMMENT,
};

struct Token {
    Token_Type type;
    std::string text;
};

inline std::vector<Token> tokenize(const char * filepath) {
    std::vector<Token> tokens;
    size_t i = 0;
    size_t line = 1;

    FileContent* file_content;
    file_read(&file_content, filepath);

    const std::string src(file_content->data, file_content->size);

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

        // (expression)
        if (c == '(') {
            int depth = 1;
            const size_t start = ++i;

            while (i < src.size() && depth > 0) {
                if (src[i] == '(') depth++;
                else if (src[i] == ')') depth--;
                i++;
            }

            if (depth > 0) throw std::runtime_error("DSL line" + std::to_string(line) + " error : Wrong bracket stuffy");

            std::string expr = src.substr(start, i - start - 1);
            tokens.push_back({Token_Type::T_EXP, expr});
            continue;
        }

        // number
        if (isdigit(c) || (c == '-' && i + 1 < src.size() && isdigit(src[i + 1]))) {
            size_t start = i++;

            while (i < src.size() && (isdigit(src[i]) || src[i] == '.'))
                i++;

            tokens.push_back({
                Token_Type::T_NUM,
                src.substr(start, i - start)
            });
            continue;
        }

        // --- identifier ---
        if (isalpha(c) || c == '_') {
            size_t start = i;

            while (i < src.size() && (isalnum(src[i]) || src[i] == '_'))
                i++;

            tokens.push_back({
                Token_Type::T_IDENT,
                src.substr(start, i - start)
            });
            continue;
        }

        // --- unknown character ---
        throw std::runtime_error("DSL line" + std::to_string(line) +  " error : What did you do?");
    }

    tokens.push_back({Token_Type::T_END, ""});
    return tokens;
}