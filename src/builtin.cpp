#include "common.hpp"

static shared_ptr<List> getArgs(Env &env, shared_ptr<Symbol>& whichArgs = Function::args) {
    auto args = env.get(whichArgs);
    if (!instanceof<List>(args)) {
        auto argsList = make_shared<List>();
        argsList->lst.push_back(args);
        return argsList;
    }
    return dynamic_pointer_cast<List>(args);
}

// Remove the numerical prefix (if any) from the RTTI type name
static char* cleanTypeName(const char *str) {
    char* str_end;
    strtod(str, &str_end);
    return str_end;
}

template <typename expectedType>
static string typeErrorMsg(shared_ptr<Value> val_ptr) {
    Value &val = *val_ptr;
    const char *expectedTypeName = cleanTypeName(typeid(expectedType).name());
    const char *receivedTypeName = cleanTypeName(typeid(val).name());

    stringstream errMsg;
    errMsg << "Expected a value of type " << expectedTypeName
           << ", but instead got the value `" << val
           << "` of type " << receivedTypeName << ".";
    return errMsg.str();
}

template <typename targetType>
static shared_ptr<targetType> vCast(shared_ptr<Value> val) {
    shared_ptr<targetType> result = dynamic_pointer_cast<targetType>(val);
    if (result == nullptr)
        throw Error(typeErrorMsg<targetType>(val));
    return result;
}

struct Fn : Function {
    vector<shared_ptr<Symbol>> params;
    shared_ptr<Value> expr;

    Fn(vector<shared_ptr<Symbol>> &_params, shared_ptr<Value> _expr) : params(move(_params)), expr(_expr) {}

    shared_ptr<Value> apply(Env &env) override {
        auto args = getArgs(env)->lst;

        if (args.size() != params.size()) {
            throw Error("This functions expects " + toString(params.size()) + " arguments.");
        }

        Env fnEnv(env);
        auto param = params.begin();
        for (auto &arg : args) {
            fnEnv.assign(*param, eval(arg, env));
            param++;
        }

        return eval(expr, fnEnv);
    }
};

struct FnDefinition : Function {
    shared_ptr<Value> apply(Env &env) override {
        auto args = getArgs(env)->lst;

        if (args.size() != 2)
            throw Error("A lambda must have two arguments: (fn (a b c) expr)");

        auto param_names_list = args.front();
        args.pop_front();

        auto fn_expr = args.front();
        args.pop_front();

        if (!instanceof<List>(param_names_list))
            throw Error("The lambda parameters must be a list.");

        vector<shared_ptr<Symbol>> params;
        for (auto param_name : dynamic_pointer_cast<List>(param_names_list)->lst) {
            if (!instanceof<Symbol>(param_name))  {
                throw Error("The lambda parameter names must all be symbols. This is not a symbol: " + toString(*param_name));
            }
            params.push_back(dynamic_pointer_cast<Symbol>(param_name));
        }

        return make_shared<Fn>(params, fn_expr);
    }
};

struct SemicolonFunction : Function {
    shared_ptr<Value> apply(Env &env) override {
        auto lhs = getArgs(env, Function::lhs);
        auto rhs = getArgs(env);

        auto lhsResult = eval(lhs, env);
        if (rhs->lst.empty()) {
            return lhsResult;
        }
        env.assign(Symbol::create("_"), lhsResult);
        return eval(rhs, env);
    }
};

struct AddFunction : Function {
    shared_ptr<Value> apply(Env &env) override {
        auto args = getArgs(env);
        auto lhs = getArgs(env, Function::lhs);
        args->lst.splice(args->lst.begin(), lhs->lst);

        Real sum(0.0f);
        for (auto &k : args->lst) {
            auto x = eval(k, env);
            Real& val = *vCast<Real>(x);

            sum = sum + val;
        }
        return make_shared<Real>(sum);
    }
};

struct SubFunction : Function {
    shared_ptr<Value> apply(Env &env) override {
        auto args = getArgs(env);
        auto lhs = getArgs(env, Function::lhs);
        unsigned long pivot = lhs->lst.size();
        args->lst.splice(args->lst.begin(), lhs->lst);

        Real left_sum(0.0f);
        Real right_sum(0.0f);

        for (auto &k : args->lst) {
            auto x = eval(k, env);
            Real& val = *vCast<Real>(x);

            if (pivot-- > 0)
                left_sum = left_sum + val;
            else
                right_sum = right_sum + val;
        }

        return make_shared<Real>(left_sum - right_sum);
    }
};

struct MulFunction : Function {
    shared_ptr<Value> apply(Env &env) override {
        auto args = getArgs(env);
        auto lhs = getArgs(env, Function::lhs);
        args->lst.splice(args->lst.begin(), lhs->lst);

        Real product(1.0f);
        for (auto &k : args->lst) {
            auto x = eval(k, env);
            Real& val = *vCast<Real>(x);

            product = product * val;
        }
        return make_shared<Real>(product);
    }
};

struct DivFunction : Function {
    shared_ptr<Value> apply(Env &env) override {
        auto args = getArgs(env);
        auto lhs = getArgs(env, Function::lhs);
        unsigned long pivot = args->lst.size();
        args->lst.splice(args->lst.begin(), lhs->lst);

        Real left_product(1.0f);
        Real right_product(1.0f);

        for (auto &k : args->lst) {
            auto x = eval(k, env);
            Real& val = *vCast<Real>(x);

            if (pivot-- > 0)
                left_product = left_product * val;
            else
                right_product = right_product * val;
        }

        return make_shared<Real>(left_product / right_product);
    }
};

struct IfFunction : Function {
    shared_ptr<Value> apply(Env &env) override {
        auto args = getArgs(env);
        auto lhs = getArgs(env, Function::lhs);
        args->lst.splice(args->lst.begin(), lhs->lst);

        if (args->lst.size() != 3)
            throw Error("The ? function expects exactly 3 arguments: cond, conseq, alt.");

        auto cond = args->lst.front();
        args->lst.pop_front();

        auto conseq = args->lst.front();
        args->lst.pop_front();

        auto alt = args->lst.front();
        args->lst.pop_front();

        auto cond_result = eval(cond, env);

        // TODO: create Boolean type, require/expect it here

        auto result_bool = (*vCast<Real>(cond_result))() != 0.0;

        if (result_bool)
            return eval(conseq, env);
        else
            return eval(alt, env);
    }
};

struct AssignFunction : Function {
    shared_ptr<Value> apply(Env &env) override {
        auto args = getArgs(env);
        auto lhs = getArgs(env, Function::lhs);
        args->lst.splice(args->lst.begin(), lhs->lst);

        if (args->lst.size() != 2)
            throw Error("The assignment function expects exactly 2 arguments: var, val.");

        auto var_name = args->lst.front();
        args->lst.pop_front();

        auto var_val = args->lst.front();
        args->lst.pop_front();

        if (!instanceof<Symbol>(var_name))
            throw Error("The variable name must be a symbol.");

        auto var = dynamic_pointer_cast<Symbol>(var_name);
        auto val = eval(var_val, env);

        Env* outerEnv = env.outerEnv;
        if (outerEnv) {
            outerEnv->assign(var, val);
        }

        return var_name;
    }
};

struct StrMapFunction : Function {
    shared_ptr<Value> apply(Env &env) override {
        auto args = getArgs(env);
        auto lhs = getArgs(env, Function::lhs);
        args->lst.splice(args->lst.begin(), lhs->lst);
        auto strMap = make_shared<StrMap>();
        for (auto &entry : args->lst) {
            auto errMsg = "A map must be made up a list of pairs of strings mapping to value, like (('a' 1) ('b' 2)).";
            if (!instanceof<List>(entry))
                throw Error(errMsg);
            auto entryList = dynamic_pointer_cast<List>(entry)->lst;
            if (entryList.size() != 2)
                throw Error(errMsg);
            auto key = entryList.front();
            auto value = entryList.back();
            if (!instanceof<UserString>(key))
                throw Error(errMsg);
            auto keyStr = dynamic_pointer_cast<UserString>(key)->text;
            (strMap->strMap)[keyStr] = value;
        }
        return strMap;
    }
};

struct PrintFunction : Function {
    shared_ptr<Value> apply(Env &env) override {
        auto args = getArgs(env);
        auto lhs = getArgs(env, Function::lhs);
        args->lst.splice(args->lst.begin(), lhs->lst);
        string result = toString(*args);
        cout << result << endl;
        return make_shared<List>();
    }
};

struct ExitFunction : Function {
    shared_ptr<Value> apply(Env &env) override {
        throw ExitNow(0);
    }
};

static void def(Env& env, const string& name, shared_ptr<Function> fn, float precedence, bool specialForm) {
    auto symbol = Symbol::create(name);
    env.assign(symbol, fn);
    fn->symbol = symbol;
    fn->precedence = precedence;
    fn->specialForm = specialForm;
}

float Function::defaultPrecedence = 19;

template <class T>
static void def(Env* env, const string& name, float precedence = Function::defaultPrecedence, bool specialForm = false) {
    def(*env, name, make_shared<T>(), precedence, specialForm);
}

Env::Env() : outerEnv(nullptr) {
    def<AddFunction>(this, "+", 13);
    def<SubFunction>(this, "-", 13.1);
    def<MulFunction>(this, "*", 14);
    def<DivFunction>(this, "/", 14);
    def<IfFunction>(this, "?", Function::defaultPrecedence, true);
    def<FnDefinition>(this, "fn", Function::defaultPrecedence, true);
    def<SemicolonFunction>(this, ";", 0, true);
    def<AssignFunction>(this, "=", 3, true);
    def<StrMapFunction>(this, "map");
    def<PrintFunction>(this, "print");
    def<ExitFunction>(this, "quit");
}
