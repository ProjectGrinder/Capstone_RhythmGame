#pragma once
#include "parser.h"
#include <unordered_set>
#include "game/components/battle/bullet_data.h"
#include "game/components/battle/battlestate.h"

namespace DSL
{
    using Value = std::variant<float, bool>;

    struct BuiltinInfo
    {
        int min_args;
        int max_args;
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

    class Validator
    {
    public:
        struct Scope
        {
            std::unordered_set<std::string> variables;
        };
        void validate(const AST& ast);

    private:
        Scope scope;

        void validate_stmt(const Statement& stmt, Scope &scope, const size_t &line);
        void validate_expr(const Expr& expr, Scope &scope, const size_t &line);

        static void validate_variable(const VariableExpr &var, const Scope &scope, const size_t &line);
        void validate_assign(const AssignStatement& stmt,Scope &scope, const size_t &line);
        void validate_ascent(const AscentStatement& stmt, const Scope &scope, const size_t &line);
        void validate_function(const FunctionExpr& func, Scope &scope, const size_t &line);
    };

    struct RuntimeScope
    {
        std::unordered_map<std::string, Value> variables;
    };
    class Evaluator
    {
    public:
        Value eval(const Expr& expr, RuntimeScope& scope);
        static float as_number(const Value &v);
        static bool as_bool(const Value& v);

    private:
        Value eval_expr(const Expr& expr, RuntimeScope& scope);
        Value eval_function(const FunctionExpr& func, RuntimeScope& scope);
        Value eval_binary(const BinaryExpr& expr, RuntimeScope& scope);
    };



    class BulletHellCompiler
    {
    public:
        RuntimeScope global;
        Game::Battle::BulletLoader extract_bullet_from_file(const char * filepath);
        Game::Battle::BulletLoader get_bullet_reg();
        Game::Battle::BulletLoader get_bullet_reg(const char * filepath);

    private:
        // Cache
        const char *filepath = nullptr;
        std::vector<Token> tokens;
        AST ast;
        Evaluator evaluator;

        Game::Battle::BulletLoader bullet_reg;
        size_t pos = 0;
        void compile_statement(const Statement& stmt, RuntimeScope& scope);
        void compile_if(const IfStatement& stmt, RuntimeScope& scope);
        void compile_ascent(const AscentStatement &stmt, RuntimeScope &scope);
        void compile_spawn(const SpawnStatement &stmt, RuntimeScope &scope);
        void compile_assign(const AssignStatement& stmt, RuntimeScope& scope);
    };
}


