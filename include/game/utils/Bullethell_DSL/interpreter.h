#pragma once
#include <unordered_set>
#include "game/components/battle/battlestate.h"
#include "game/components/battle/bullet_data.h"
#include "parser.h"

namespace DSL
{
    using Value = std::variant<float, bool>;

    struct BuiltinInfo
    {
        int min_args;
        int max_args;
    };

    inline const std::unordered_set<std::string> BUILTIN_CONFIG = {
        "@BPM"
    };

    inline const std::unordered_set<std::string> BUILTIN_CONSTANTS = {
        "BOX_LEFT", "BOX_RIGHT", "BOX_UP", "BOX_DOWN", "HALF_WIDTH", "HALF_HEIGHT", "BEAT"
    };

    inline const std::unordered_map<std::string, BuiltinInfo> BUILTIN_FUNCTIONS = {
        {"rand", {0,2}},
        {"sin", {1,1}},
        {"cos", {1,1}},
        {"tan", {1,1}},
        {"sqrt", {1,1}},
        {"abs", {1,1}},
        {"round", {1,1}},
        {"ceil", {1,1}},
        {"floor", {1,1}},
    };
    struct Scope
    {
        Scope* parent = nullptr;
        std::unordered_map<std::string, Value> variables;
        Value* find(const std::string &name)
        {
            const auto it = variables.find(name);

            if (it != variables.end())
                return &it->second;

            if (parent)
                return parent->find(name);

            return nullptr;
        }
        const Value* find(const std::string& name) const{
            const auto it = variables.find(name);

            if (it != variables.end())
                return &it->second;

            if (parent)
                return parent->find(name);

            return nullptr;
        }
    };
    class Validator
    {
    public:
        void validate(const AST& ast);
        Scope scope;

    private:
        void validate_stmt(const Statement& stmt, Scope &scope);
        void validate_expr(const Expr& expr, Scope &scope, const size_t &line);

        static void validate_variable(const VariableExpr &var, Scope &scope, const size_t &line);
        void validate_assign(const AssignStatement& stmt,Scope &scope, const size_t &line);
        void validate_ascent(const AscentStatement& stmt, Scope &scope, const size_t &line);
        void validate_function(const FunctionExpr& func, Scope &scope, const size_t &line);
    };

    class BulletHellCompiler
    {
    public:
        Scope global;
        Game::Battle::BulletLoader extract_bullets(const std::string &content);


    private:
        // Cache
        std::vector<Token> tokens;
        AST ast;
        Game::Battle::BulletLoader bullet_reg;
        size_t pos = 0;
        float bpm = -1;
        void compile_statement(const Statement& stmt, Scope& scope);
        void compile_if(const IfStatement& stmt, Scope& scope, const size_t &line);
        void compile_ascent(const AscentStatement &stmt, Scope &scope, const size_t &line);
        void compile_spawn(const SpawnStatement &stmt, Scope &scope, const size_t &line);
        void compile_assign(const AssignStatement& stmt, Scope& scope, const size_t &line);
        Value eval_expr(const Expr& expr, Scope& scope, const size_t &line);
        Value eval_function(const FunctionExpr& func, Scope& scope, const size_t &line);
        Value eval_binary(const BinaryExpr& expr, Scope& scope, const size_t &line);
    };
}


