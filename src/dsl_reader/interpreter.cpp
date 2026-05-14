#include <utility>

#include "game/utils/Bullethell_DSL/interpreter.h"
#include "game/utils/physics_util.h"
#include "game/utils/constant.h"

using namespace DSL;

void Validator::validate_function(const FunctionExpr& func, Scope &scope, const size_t &line)
{
    const auto it = BUILTIN_FUNCTIONS.find(func.name);

    if (it == BUILTIN_FUNCTIONS.end())
        throw std::runtime_error("DSL line " + std::to_string(line) + " error : No built-in function for this");

    const auto &[min_args, max_args] = it->second;

    if (const int count = static_cast<int>(func.args.size()); count < min_args || count > max_args)
        throw std::runtime_error(
                "DSL line " + std::to_string(line) + " error : This function accept " + std::to_string(max_args) +
                " arguments not " + std::to_string(count));

    for (const auto &arg: func.args)
        validate_expr(*arg, scope, line);
}

void Validator::validate_variable(const VariableExpr &var, const Scope &scope, const size_t &line)
{
    if (!(BUILTIN_CONSTANTS.contains(var.name) || scope.variables.contains(var.name)))
        throw std::runtime_error("DSL line " + std::to_string(line) + " error : Unknown variable " + var.name);
}

void Validator::validate_assign(const AssignStatement &stmt, Scope &scope, const size_t &line)
{
    if (BUILTIN_FUNCTIONS.contains(stmt.name) && BUILTIN_CONSTANTS.contains(stmt.name))
        throw std::runtime_error("DSL line " + std::to_string(line) + " error : Trying to assign " + stmt.name);

    validate_expr(*stmt.value, scope, line);

    scope.variables.insert(stmt.name);
}

void Validator::validate_ascent(const AscentStatement &stmt, const Scope &parent, const size_t &line)
{
    Scope local = parent;

    local.variables.insert(stmt.iterator);

    validate_expr(*stmt.start, local, line);
    validate_expr(*stmt.end, local, line);

    for (const auto &s: stmt.body)
        validate_stmt(*s, local,line);
}

void Validator::validate_expr(const Expr &expr, Scope &scope, const size_t &line)
{
    std::visit([&](const auto &e)
    {
        using T = std::decay_t<decltype(e)>;

        if constexpr (std::is_same_v<T, VariableExpr>)
            validate_variable(e, scope);
        else if constexpr (std::is_same_v<T, UnaryExpr>)
            validate_expr(*e.value, scope);
        else if constexpr (std::is_same_v<T, BinaryExpr>)
        {
            validate_expr(*e.left, scope);
            validate_expr(*e.right, scope);
        }
        else if constexpr (std::is_same_v<T, FunctionExpr>)
            validate_function(e, scope);
    },
    expr.expr);
}

void Validator::validate_stmt(const Statement& stmt, Scope& scope, const size_t &line)
{
    std::visit([&](const auto& s)
    {
        using T = std::decay_t<decltype(s)>;

        if constexpr (std::is_same_v<T, AssignStatement>)
            validate_assign(s, scope);

        else if constexpr (std::is_same_v<T, SpawnStatement>)
        {
            for (const auto& arg : s.args)
                validate_expr(*arg, scope);
        }

        else if constexpr (std::is_same_v<T, BlockStatement>)
        {
            Scope local = scope;

            for (const auto& sub_stmt : s.body)
                validate_stmt(*sub_stmt, local);
        }

        else if constexpr (std::is_same_v<T, AscentStatement>)
            validate_ascent(s, scope);

        else if constexpr (std::is_same_v<T, IfStatement>)
        {
            for (const auto& branch : s.branches)
            {
                Scope local = scope;

                validate_expr(*branch.condition, local);

                for (const auto& sub_stmt : branch.body)
                    validate_stmt(*sub_stmt, local);
            }

            Scope local = scope;

            for (const auto& sub_stmt : s.else_body)
                validate_stmt(*sub_stmt, local);
        }

    }, stmt.stmt);
}

void Validator::validate(const AST &ast)
{
    Scope global;

    for (const auto &stmt: ast.statements)
        validate_stmt(*stmt, global, stmt->line);
}

float Evaluator::as_number(const Value& v)
{
    if (std::holds_alternative<float>(v)) return std::get<float>(v);
    if (std::holds_alternative<bool>(v)) return std::get<bool>(v) ? 1.f : 0.f;
    return 0.f;
}

bool Evaluator::as_bool(const Value& v)
{
    if (std::holds_alternative<float>(v)) return std::get<float>(v)!=0;
    if (std::holds_alternative<bool>(v)) return std::get<bool>(v);
    return false;
}

Value Evaluator::eval_function(const FunctionExpr& func, RuntimeScope& scope)
{
    std::vector<Value> args;

    for (const auto& arg : func.args)
        args.push_back(eval_expr(*arg, scope));

    if (func.name == "sin") return std::sin(as_number(args[0]));
    if (func.name == "cos") return std::cos(as_number(args[0]));
    if (func.name == "sqrt") return std::sqrt(as_number(args[0]));
    if (func.name == "abs") return std::abs(as_number(args[0]));
    if (func.name == "rand")
    {
        if (args.empty()) return Game::Physics::rand_float(0.f,1.f);

        if (args.size() == 2) return Game::Physics::rand_float(as_number(args[0]), as_number(args[1]));
    }
    return {};
}

Value Evaluator::eval_binary(const BinaryExpr &expr, RuntimeScope& scope)
{
    Value left = eval_expr(*expr.left, scope);
    Value right = eval_expr(*expr.right, scope);

    if (expr.op == Op_Type::O_ADD)
        return as_number(left) + as_number(right);
    if (expr.op == Op_Type::O_SUB)
        return as_number(left) - as_number(right);
    if (expr.op == Op_Type::O_MUL)
        return as_number(left) * as_number(right);
    if (expr.op == Op_Type::O_DIV)
    {
        if (as_number(right) == 0) throw std::runtime_error("divide by zero somewhere IDK lol");
        return as_number(left) / as_number(right);
    }
    if (expr.op == Op_Type::O_MOD)
    {
        if (as_number(right) == 0) throw std::runtime_error("modulo by zero somewhere IDK lol");
        return static_cast<float>(static_cast<int>(as_number(left)) % static_cast<int>(as_number(right)));
    }

    if (expr.op == Op_Type::O_EQ)
        return as_number(left) == as_number(right);
    if (expr.op == Op_Type::O_LT)
        return as_number(left) < as_number(right);
    if (expr.op == Op_Type::O_GT)
        return as_number(left) > as_number(right);
    if (expr.op == Op_Type::O_LTE)
        return as_number(left) <= as_number(right);
    if (expr.op == Op_Type::O_GTE)
        return as_number(left) >= as_number(right);
    if (expr.op == Op_Type::O_AND)
    {
        if (!as_bool(left))
            return false;
        return as_bool(left) && as_bool(right);
    }
    if (expr.op == Op_Type::O_OR)
    {
        if (as_bool(left))
            return true;
        return as_bool(left) || as_bool(right);
    }
    return {};
}

Value Evaluator::eval_expr(const Expr& expr, RuntimeScope& scope)
{
    return std::visit([&](const auto& e) -> Value
    {
        using T = std::decay_t<decltype(e)>;

        if constexpr (std::is_same_v<T, NumberExpr> || std::is_same_v<T, BooleanExpr>) return e.value;
        if constexpr (std::is_same_v<T, VariableExpr>)
        {
            if (e.name == "BOX_LEFT") return -Game::BOX_SIZE + Game::HALF_WIDTH / 32;
            if (e.name == "BOX_RIGHT") return Game::BOX_SIZE - Game::HALF_WIDTH / 32;
            if (e.name == "BOX_UP") return Game::BOX_SIZE + Game::HALF_HEIGHT / 32 - Game::HALF_HEIGHT/ 3;
            if (e.name == "BOX_DOWN") return -Game::BOX_SIZE-Game::HALF_HEIGHT/32-Game::HALF_HEIGHT/3;
            if (e.name == "HALF_WIDTH") return Game::HALF_WIDTH;
            if (e.name == "HALF_HEIGHT") return Game::HALF_HEIGHT;

            const auto it = scope.variables.find(e.name);
            if (it == scope.variables.end())
                return {};
            return it->second;
        }
        if constexpr (std::is_same_v<T, UnaryExpr>)
        {
            Value value = eval_expr(*e.value, scope);
            if (e.op == Op_Type::O_SUB) return -as_number(value);
            if (e.op == Op_Type::O_ADD) return +as_number(value);
            if (e.op == Op_Type::O_NOT) return !as_bool(value);

            return value;
        }
        if constexpr (std::is_same_v<T, BinaryExpr>)
        {
            return eval_binary(e, scope);
        }
        if constexpr (std::is_same_v<T, FunctionExpr>)
        {
            return eval_function(e, scope);
        }

        return {};

    }, expr.expr);
}

Value Evaluator::eval(const Expr &expr, RuntimeScope& scope)
{
    return eval_expr(expr, scope);
}


void BulletHellCompiler::compile_if(
    const IfStatement& stmt,
    RuntimeScope& scope)
{
    for (auto& branch : stmt.branches)
    {
        if (Evaluator::as_bool(evaluator.eval(*branch.condition, scope)))
        {
            RuntimeScope child = scope;

            for (auto& s : branch.body)
                compile_statement(*s, child);

            return;
        }
    }

    if (!stmt.else_body.empty())
    {
        RuntimeScope child = scope;

        for (auto& s : stmt.else_body)
            compile_statement(*s, child);
    }
}

void BulletHellCompiler::compile_ascent(const AscentStatement &stmt, RuntimeScope &scope)
{
    const int start = static_cast<int>(Evaluator::as_number(evaluator.eval(*stmt.start, scope)));
    const int end   = static_cast<int>(Evaluator::as_number(evaluator.eval(*stmt.end, scope)));

    for (int i = start; i < end; i++)
    {
        RuntimeScope child = scope;
        child.variables[stmt.iterator] = static_cast<float>(i);

        for (auto& st : stmt.body)
            compile_statement(*st, child);
    }
}

void BulletHellCompiler::compile_assign(const AssignStatement& stmt, RuntimeScope& scope)
{
    const Value v = evaluator.eval(*stmt.value, scope);
    auto &var = scope.variables[stmt.name];
    float num = Evaluator::as_number(v);

    switch(stmt.op)
    {
    case Op_Type::O_ASSIGN:
        scope.variables[stmt.name] = num;
        break;

    case Op_Type::O_ADD_ASSIGN:
        if (!std::holds_alternative<float>(v)) var = 0.f;
        std::get<float>(var) += num;
        break;

    case Op_Type::O_SUB_ASSIGN:
        if (!std::holds_alternative<float>(v)) var = 0.f;
        std::get<float>(var) -= num;
        break;
    default:
        scope.variables[stmt.name] = num;
        break;
    }
}

void BulletHellCompiler::compile_statement(const Statement& stmt, RuntimeScope& scope)
{
    std::visit([&](auto&& s)
    {
        using T = std::decay_t<decltype(s)>;

        if constexpr (std::is_same_v<T, SpawnStatement>)
            compile_spawn(s, scope);

        else if constexpr (std::is_same_v<T, AssignStatement>)
            compile_assign(s, scope);

        else if constexpr (std::is_same_v<T, IfStatement>)
            compile_if(s, scope);

        else if constexpr (std::is_same_v<T, AscentStatement>)
            compile_ascent(s, scope);

        else if constexpr (std::is_same_v<T, BlockStatement>)
        {
            for (auto& st : s.body)
                compile_statement(*st, scope);
        }

    }, stmt.stmt);
}

void BulletHellCompiler::compile_spawn(const SpawnStatement& stmt, RuntimeScope& scope)
{
    std::vector<float> args;

    for (auto& e : stmt.args)
    {
        Value v = evaluator.eval(*e, scope);
        args.push_back(Evaluator::as_number(v));
    }

    // Scalability is not real. I'm sleep-deprived now.
    if (args.size() == 4) bullet_reg.CreateBullet(static_cast<int>(args[0]),Game::Battle::BulletData{static_cast<int>(args[1]),args[2],args[3]});
    if (args.size() == 5) bullet_reg.CreateBullet(static_cast<int>(args[0]),Game::Battle::BulletData{static_cast<int>(args[1]),args[2],args[3],args[4]});
    if (args.size() == 6) bullet_reg.CreateBullet(static_cast<int>(args[0]),Game::Battle::BulletData{static_cast<int>(args[1]),args[2],args[3],args[4],args[5]});
    if (args.size() == 7) bullet_reg.CreateBullet(static_cast<int>(args[0]),Game::Battle::BulletData{static_cast<int>(args[1]),args[2],args[3],args[4], args[5],args[6]});
    if (args.size() == 8) bullet_reg.CreateBullet(static_cast<int>(args[0]),Game::Battle::BulletData{static_cast<int>(args[1]),args[2],args[3],args[4],args[5], args[6],args[7]});
    if (args.size() == 9) bullet_reg.CreateBullet(static_cast<int>(args[0]),Game::Battle::BulletData{static_cast<int>(args[1]),args[2],args[3],args[4],args[5], args[6],args[7] ,static_cast<unsigned short>(args[8])});
    if (args.size() == 10) bullet_reg.CreateBullet(static_cast<int>(args[0]),Game::Battle::BulletData{static_cast<int>(args[1]),args[2],args[3],args[4],args[5], args[6],args[7] ,static_cast<unsigned short>(args[8]), args[9]});
    if (args.size() == 11) bullet_reg.CreateBullet(static_cast<int>(args[0]),Game::Battle::BulletData{static_cast<int>(args[1]),args[2],args[3],args[4],args[5], args[6],args[7] ,static_cast<unsigned short>(args[8]), args[9], args[10]});
    if (args.size() == 12) bullet_reg.CreateBullet(static_cast<int>(args[0]),Game::Battle::BulletData{static_cast<int>(args[1]),args[2],args[3],args[4],args[5], args[6],args[7] ,static_cast<unsigned short>(args[8]), args[9], args[10], args[11]});
    if (args.size() == 13) bullet_reg.CreateBullet(static_cast<int>(args[0]),Game::Battle::BulletData{static_cast<int>(args[1]),args[2],args[3],args[4],args[5], args[6],args[7] ,static_cast<unsigned short>(args[8]), args[9], args[10], args[11], args[12]});
}

Game::Battle::BulletLoader BulletHellCompiler::extract_bullet_from_file(const char *filepath)
{
    this->filepath = filepath;
    tokens = tokenize(filepath);
    ast = parser(tokens);
    bullet_reg = {};

    RuntimeScope global;

    for (auto& stmt : ast.statements)
    {
        compile_statement(*stmt, global);
    }

    return bullet_reg;
}
Game::Battle::BulletLoader BulletHellCompiler::get_bullet_reg()
{
    if (this->filepath != nullptr && this->bullet_reg.batches.empty()) return extract_bullet_from_file(filepath);
    return this->bullet_reg;
}

Game::Battle::BulletLoader BulletHellCompiler::get_bullet_reg(const char *filepath)
{
    if (this->filepath == nullptr || this->bullet_reg.batches.empty())
        return extract_bullet_from_file(filepath);
    return this->bullet_reg;
}

