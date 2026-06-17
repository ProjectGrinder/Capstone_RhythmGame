#include <utility>

#include "../../include/game/utils/DSL/bullethell/interpreter.h"

#include "game/utils/constant.h"
#include "game/utils/physics_util.h"
#include "utils/print_debug.h"

using namespace DSL;

void Validator::validate_function(const FunctionExpr& func, Scope &local, const size_t &line)
{
    const auto it = BUILTIN_FUNCTIONS.find(func.name);

    if (it == BUILTIN_FUNCTIONS.end())
        throw std::runtime_error("DSL line " + std::to_string(line) + " error : No built-in function for " + func.name);

    const auto &[min_args, max_args] = it->second;

    if (const int count = static_cast<int>(func.args.size()); count < min_args || count > max_args)
        throw std::runtime_error(
        "DSL line " + std::to_string(line) + " error : This function accept " + std::to_string(max_args) +
                " arguments not " + std::to_string(count));

    for (const auto &arg: func.args)
        validate_expr(*arg, local, line);
}

void Validator::validate_variable(const VariableExpr &var, Scope &local, const size_t &line)
{
    if (!(BUILTIN_CONSTANTS.contains(var.name) || local.find(var.name)))
        throw std::runtime_error("DSL line " + std::to_string(line) + " error : Unknown variable " + var.name);
}

void Validator::validate_assign(const AssignStatement &stmt, Scope &local, const size_t &line)
{
    if (BUILTIN_FUNCTIONS.contains(stmt.name) || BUILTIN_CONSTANTS.contains(stmt.name))
        throw std::runtime_error("DSL line " + std::to_string(line) + " error : Trying to assign constant " + stmt.name);

    if (stmt.op != Op_Type::O_ASSIGN && !local.find(stmt.name))
        throw std::runtime_error("DSL line " + std::to_string(line) + " error : Variable " + stmt.name + " not initialized");

    validate_expr(*stmt.value, local, line);

    if (stmt.op == Op_Type::O_ASSIGN)
        local.variables[stmt.name] = 0.f;
}

void Validator::validate_ascent(const AscentStatement &stmt, Scope &parent, const size_t &line)
{
    Scope local;
    local.parent = &parent;

    local.variables[stmt.iterator] = 0.f;

    validate_expr(*stmt.start, local, line);
    validate_expr(*stmt.end, local, line);

    for (const auto &s: stmt.body)
        validate_stmt(*s, local);
}

void Validator::validate_expr(const Expr &expr, Scope &local, const size_t &line)
{
    std::visit([&](const auto &e)
    {
        using T = std::decay_t<decltype(e)>;

        if constexpr (std::is_same_v<T, VariableExpr>)
            validate_variable(e, local, line);
        else if constexpr (std::is_same_v<T, UnaryExpr>)
            validate_expr(*e.value, local, line);
        else if constexpr (std::is_same_v<T, BinaryExpr>)
        {
            validate_expr(*e.left, local, line);
            validate_expr(*e.right, local, line);
        }
        else if constexpr (std::is_same_v<T, FunctionExpr>)
            validate_function(e, local, line);
    },
    expr.expr);
}

void Validator::validate_stmt(const Statement& stmt, Scope& local)
{
    std::visit([&](const auto& s)
    {
        using T = std::decay_t<decltype(s)>;

        if constexpr (std::is_same_v<T, AssignStatement>)
            validate_assign(s, local, stmt.line);

        else if constexpr (std::is_same_v<T, SpawnStatement>)
        {
            for (const auto& arg : s.args)
                validate_expr(*arg, local, stmt.line);
        }

        else if constexpr (std::is_same_v<T, BlockStatement>)
        {
            Scope local2;
            local2.parent = &local;

            for (const auto& sub_stmt : s.body)
                validate_stmt(*sub_stmt, local2);
        }

        else if constexpr (std::is_same_v<T, AscentStatement>)
            validate_ascent(s, local, stmt.line);

        else if constexpr (std::is_same_v<T, IfStatement>)
        {
            for (const auto& branch : s.branches)
            {
                Scope local2;
                local2.parent = &local;

                validate_expr(*branch.condition, local2, stmt.line);

                for (const auto& sub_stmt : branch.body)
                    validate_stmt(*sub_stmt, local2);
            }

            Scope local2;
            local2.parent = &local;

            for (const auto& sub_stmt : s.else_body)
                validate_stmt(*sub_stmt, local2);
        }

    }, stmt.stmt);
}

void Validator::validate(const AST &ast)
{
    Scope global;

    for (const auto &stmt: ast.statements)
        validate_stmt(*stmt, global);
}

float as_number(const Value& v)
{
    if (std::holds_alternative<float>(v)) return std::get<float>(v);
    if (std::holds_alternative<bool>(v)) return std::get<bool>(v) ? 1.f : 0.f;
    return 0.f;
}

bool as_bool(const Value& v)
{
    if (std::holds_alternative<float>(v)) return std::get<float>(v)!=0;
    if (std::holds_alternative<bool>(v)) return std::get<bool>(v);
    return false;
}

Value BulletHellCompiler::eval_function(const FunctionExpr& func, Scope& scope, const size_t &line)
{
    std::vector<Value> args;

    for (const auto& arg : func.args)
        args.push_back(eval_expr(*arg, scope, line));

    if (func.name == "sin") return std::sin(as_number(args[0]));
    if (func.name == "cos") return std::cos(as_number(args[0]));
    if (func.name == "sqrt")
    {
        if (as_number(args[0]) < 0) throw std::runtime_error("DSL line " + std::to_string(line) + " error : square root negative number");
        return std::sqrt(as_number(args[0]));
    }
    if (func.name == "abs") return std::abs(as_number(args[0]));
    if (func.name == "rand")
    {
        if (args.empty()) return Game::Physics::rand_float(0.f,1.f);

        if (args.size() == 2)
        {
            if (as_number(args[0]) > as_number(args[1]))
                throw std::runtime_error("DSL line " + std::to_string(line) + " rand min > rand max??");
            return Game::Physics::rand_float(as_number(args[0]), as_number(args[1]));
        }
    }
    if (func.name == "round") return std::round(as_number(args[0]));
    if (func.name == "ceil") return std::ceil(as_number(args[0]));
    if (func.name == "floor") return std::floor(as_number(args[0]));
    throw std::runtime_error("DSL line " + std::to_string(line) + " error : Unsupported function");
}

Value BulletHellCompiler::eval_binary(const BinaryExpr &expr, Scope& scope, const size_t &line)
{
    const Value left = eval_expr(*expr.left, scope, line);

    if (expr.op == Op_Type::O_AND) if (!as_bool(left)) return false;
    if (expr.op == Op_Type::O_OR)
        if (as_bool(left))
            return true;
    const Value right = eval_expr(*expr.right, scope, line);

    if (expr.op == Op_Type::O_ADD)
        return as_number(left) + as_number(right);
    if (expr.op == Op_Type::O_SUB)
        return as_number(left) - as_number(right);
    if (expr.op == Op_Type::O_MUL)
        return as_number(left) * as_number(right);
    if (expr.op == Op_Type::O_DIV)
    {
        if (as_number(right) == 0) throw std::runtime_error("DSL line " + std::to_string(line) + " error : bro divide by zero");
        return as_number(left) / as_number(right);
    }
    if (expr.op == Op_Type::O_MOD)
    {
        if (as_number(right) == 0) throw std::runtime_error("DSL line " + std::to_string(line) + " error : bro modulo by zero");
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
        return as_bool(left) && as_bool(right);
    if (expr.op == Op_Type::O_OR)
        return as_bool(left) || as_bool(right);
    throw std::runtime_error("DSL line " + std::to_string(line) + " error : Unknown operation");
}

Value BulletHellCompiler::eval_expr(const Expr& expr, Scope& scope, const size_t &line)
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
            if (e.name == "BEAT") return 60000.f/ bpm;

            const Value* v = scope.find(e.name);
            if (!v)
                throw std::runtime_error("DSL line " + std::to_string(line) + " error : Unknown variable " + e.name);
            return *v;
        }
        if constexpr (std::is_same_v<T, UnaryExpr>)
        {
            Value value = eval_expr(*e.value, scope, line);
            if (e.op == Op_Type::O_SUB) return -as_number(value);
            if (e.op == Op_Type::O_ADD) return +as_number(value);
            if (e.op == Op_Type::O_NOT) return !as_bool(value);

            return value;
        }
        if constexpr (std::is_same_v<T, BinaryExpr>)
        {
            return eval_binary(e, scope, line);
        }
        if constexpr (std::is_same_v<T, FunctionExpr>)
        {
            return eval_function(e, scope, line);
        }
    }, expr.expr);
}

void BulletHellCompiler::compile_if(
    const IfStatement& stmt,
    Scope& scope, const size_t &line)
{
    for (auto& branch : stmt.branches)
    {
        if (as_bool(eval_expr(*branch.condition, scope, line)))
        {
            Scope child;
            child.parent = &scope;

            for (auto& s : branch.body)
                compile_statement(*s, child);

            return;
        }
    }

    if (!stmt.else_body.empty())
    {
        Scope child;
        child.parent = &scope;

        for (auto& s : stmt.else_body)
            compile_statement(*s, child);
    }
}

void BulletHellCompiler::compile_ascent(const AscentStatement &stmt, Scope &scope, const size_t &line)
{
    int start = static_cast<int>(as_number(eval_expr(*stmt.start, scope, line)));
    int end   = static_cast<int>(as_number(eval_expr(*stmt.end, scope, line)));
    for (int i = start; i < end; i++)
    {
        Scope child;
        child.parent = &scope;
        child.variables[stmt.iterator] = static_cast<float>(i);

        for (auto& st : stmt.body)
            compile_statement(*st, child);
    }
}

void BulletHellCompiler::compile_assign(const AssignStatement& stmt, Scope& scope, const size_t &line)
{
    // if (stmt.op != Op_Type::O_ASSIGN && !scope.find(stmt.name))
    // {
    //     throw std::runtime_error(
    //         "DSL line " + std::to_string(line) + " error : Variable " + stmt.name + " not initialized");
    // }

    const Value v = eval_expr(*stmt.value, scope, line);
    float num = as_number(v);
    Value* var = scope.find(stmt.name);

    if (stmt.name == "@BPM")
    {
        bpm = num;
        return;
    }

    if (var)
    {
        if (stmt.op == Op_Type::O_ASSIGN) *var = num;
        else if (stmt.op == Op_Type::O_ADD_ASSIGN) *var = as_number(*var) + num;
        else if (stmt.op == Op_Type::O_SUB_ASSIGN) *var = as_number(*var) - num;
        else if (stmt.op == Op_Type::O_MUL_ASSIGN) *var = as_number(*var) * num;
        else if (stmt.op == Op_Type::O_DIV_ASSIGN)
        {
            if (num == 0) throw std::runtime_error("DSL line " + std::to_string(line) + " error : bro divide by zero");
            *var = as_number(*var) / num;
        }
        else if (stmt.op == Op_Type::O_MOD_ASSIGN)
        {
            if (num == 0) throw std::runtime_error("DSL line " + std::to_string(line) + " error : bro divide by zero");
            *var = (float)((int)as_number(*var) % (int)num);
        }
    }
    else if (stmt.op == Op_Type::O_ASSIGN)
    {
        scope.variables[stmt.name] = num;
    }
}

void BulletHellCompiler::compile_statement(const Statement& stmt, Scope& scope)
{
    std::visit([&](auto&& s)
    {
        using T = std::decay_t<decltype(s)>;

        if constexpr (std::is_same_v<T, SpawnStatement>)
            compile_spawn(s, scope ,stmt.line);

        else if constexpr (std::is_same_v<T, AssignStatement>)
            compile_assign(s, scope,stmt.line);

        else if constexpr (std::is_same_v<T, IfStatement>)
            compile_if(s, scope,stmt.line);

        else if constexpr (std::is_same_v<T, AscentStatement>)
            compile_ascent(s, scope,stmt.line);

        else if constexpr (std::is_same_v<T, BlockStatement>)
        {
            Scope child;
            child.parent = &scope;
            for (auto& st : s.body)
                compile_statement(*st, child);
        }

    }, stmt.stmt);
}

int ComputeBeat(const float bpm, const float beat)
{
    const float time_per_beat = 60000.f/ bpm;
    return static_cast<int>(std::round(beat * time_per_beat));
}

void BulletHellCompiler::compile_spawn(const SpawnStatement& stmt, Scope& scope, const size_t &line)
{
    std::vector<float> args;

    for (auto &e: stmt.args)
    {
        Value v = eval_expr(*e, scope, line);
        args.push_back(as_number(v));
    }
    if (bpm == -1)
    {
        throw std::runtime_error("DSL line " + std::to_string(line) + " error : BPM not config");
    }
    const int spawn_frame = ComputeBeat(bpm, args[0]);

    // Scalability is my passion.
    if (args.size() == 4) bullet_reg.CreateBullet(spawn_frame,Game::Battle::BulletData{static_cast<int>(args[1]),args[2],args[3]});
    else if (args.size() == 5) bullet_reg.CreateBullet(spawn_frame,Game::Battle::BulletData{static_cast<int>(args[1]),args[2],args[3],args[4]});
    else if (args.size() == 6) bullet_reg.CreateBullet(spawn_frame,Game::Battle::BulletData{static_cast<int>(args[1]),args[2],args[3],args[4],args[5]});
    else if (args.size() == 7) bullet_reg.CreateBullet(spawn_frame,Game::Battle::BulletData{static_cast<int>(args[1]),args[2],args[3],args[4], args[5],args[6]});
    else if (args.size() == 8) bullet_reg.CreateBullet(spawn_frame,Game::Battle::BulletData{static_cast<int>(args[1]),args[2],args[3],args[4],args[5], args[6],args[7]});
    else if (args.size() == 9) bullet_reg.CreateBullet(spawn_frame,Game::Battle::BulletData{static_cast<int>(args[1]),args[2],args[3],args[4],args[5], args[6],args[7] ,static_cast<unsigned short>(args[8])});
    else if (args.size() == 10) bullet_reg.CreateBullet(spawn_frame,Game::Battle::BulletData{static_cast<int>(args[1]),args[2],args[3],args[4],args[5], args[6],args[7] ,static_cast<unsigned short>(args[8]), args[9]});
    else if (args.size() == 11) bullet_reg.CreateBullet(spawn_frame,Game::Battle::BulletData{static_cast<int>(args[1]),args[2],args[3],args[4],args[5], args[6],args[7] ,static_cast<unsigned short>(args[8]), args[9], args[10]});
    else if (args.size() == 12) bullet_reg.CreateBullet(spawn_frame,Game::Battle::BulletData{static_cast<int>(args[1]),args[2],args[3],args[4],args[5], args[6],args[7] ,static_cast<unsigned short>(args[8]), args[9], args[10], args[11]});
    else if (args.size() == 13) bullet_reg.CreateBullet(spawn_frame,Game::Battle::BulletData{static_cast<int>(args[1]),args[2],args[3],args[4],args[5], args[6],args[7] ,static_cast<unsigned short>(args[8]), args[9], args[10], args[11], args[12]});
    else throw std::runtime_error("DSL line " + std::to_string(line) + " error : Invalid spawn argument count");
}

Game::Battle::BulletLoader BulletHellCompiler::extract_bullets(const std::string &content)
{
    const auto tokens = tokenize(content);
    const auto ast = parser(tokens);

    bpm = -1;
    bullet_reg = {};
    global = {};

    Validator validator;
    validator.validate(ast);

    // Currently doesn't convert beat -> millisecond
    for (auto& stmt : ast.statements)
    {
        compile_statement(*stmt, global);
    }

    // for (auto &[frame, bullets] : bullet_reg.batches)
    // {
    //     for (const auto & b : bullets)
    //         LOG_INFO("A bullet no.%d spawns at frame %d at pos (%d,%d) with speed %d angle %d acc %d wvel %d with pattern %d",
    //             b.bullet_id, frame, (int)b.movement_data.posX, (int)b.movement_data.posY, (int)b.movement_data.vel, (int)b.movement_data.rot,
    //             (int)b.movement_data.acc, (int)b.movement_data.wvel, b.pattern.sequenceID);
    //
    // }

    return bullet_reg;
}
