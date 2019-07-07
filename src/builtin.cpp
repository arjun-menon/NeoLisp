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
            fnEnv.assign(*param, env.eval(arg));
            param++;
        }

        return fnEnv.eval(expr);
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

        auto lhsResult = env.eval(lhs);
        if (rhs->lst.empty()) {
            return lhsResult;
        }
        env.assign(Symbol::create("_"), lhsResult);
        return env.eval(rhs);
    }
};

struct AddFunction : Function {
    shared_ptr<Value> apply(Env &env) override {
        auto args = getArgs(env);
        auto lhs = getArgs(env, Function::lhs);
        args->lst.splice(args->lst.begin(), lhs->lst);

        Real sum(0.0f);
        for (auto &k : args->lst) {
            auto x = env.eval(k);
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
            auto x = env.eval(k);
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
            auto x = env.eval(k);
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
            auto x = env.eval(k);
            Real& val = *vCast<Real>(x);

            if (pivot-- > 0)
                left_product = left_product * val;
            else
                right_product = right_product * val;
        }

        return make_shared<Real>(left_product / right_product);
    }
};

struct QuoteFunction : Function {
    shared_ptr<Value> apply(Env &env) override {
        auto args = getArgs(env);
        auto lhs = getArgs(env, Function::lhs);
        args->lst.splice(args->lst.begin(), lhs->lst);

        if (args->lst.size() != 1)
            throw Error("The quote function expects exactly one argument.");

        auto expr = args->lst.front();
        return expr;
    }
};

struct RevalFunction : Function {
    shared_ptr<Value> apply(Env &env) override {
        auto args = getArgs(env);
        auto lhs = getArgs(env, Function::lhs);
        args->lst.splice(args->lst.begin(), lhs->lst);

        if (args->lst.size() != 1)
            throw Error("The eval function expects exactly one argument.");

        auto expr = args->lst.front();
        return env.eval(env.eval(expr));
    }
};

struct IfFunction : Function {
    shared_ptr<Value> apply(Env &env) override {
        auto args = getArgs(env);
        auto lhs = getArgs(env, Function::lhs);
        args->lst.splice(args->lst.begin(), lhs->lst);

        if (args->lst.size() != 3)
            throw Error("The if function expects exactly 3 arguments: cond, conseq, alt.");

        auto cond = args->lst.front();
        args->lst.pop_front();

        auto conseq = args->lst.front();
        args->lst.pop_front();

        auto alt = args->lst.front();
        args->lst.pop_front();

        auto cond_result = env.eval(cond);

        // TODO: create Boolean type, require/expect it here

        auto result_bool = (*vCast<Real>(cond_result))() != 0.0;

        if (result_bool)
            return env.eval(conseq);
        else
            return env.eval(alt);
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
        auto val = env.eval(var_val);

        Env* outerEnv = env.outerEnv;
        if (outerEnv) {
            outerEnv->assign(var, val);
        }

        return var_name;
    }
};

struct MapFunction : Function {
    shared_ptr<Value> apply(Env &env) override {
        auto args = getArgs(env);
        auto lhs = getArgs(env, Function::lhs);
        args->lst.splice(args->lst.begin(), lhs->lst);
        auto symMap = make_shared<SymbolMap>();
        for (auto &entry : args->lst) {
            auto errMsg = "A map must be made up of a list of pairs of symbols and values, like (map (a 1) (b 2)).";
            auto entryList = dynamic_pointer_cast<List>(entry);
            if (!entryList)
                throw Error(errMsg);
            auto l = entryList->lst;
            if (l.size() != 2)
                throw Error(errMsg);

            auto key = l.front();
            auto keySym = dynamic_pointer_cast<Symbol>(key);
            if (!keySym)
                throw Error(errMsg + string(" `") + toString(*key) + string("` is not a symbol."));

            auto valExpr = l.back();
            auto val = env.eval(valExpr);

            (symMap->entries)[keySym] = val;
        }
        return symMap;
    }
};

struct ImportFunction : Function {
    shared_ptr<Value> apply(Env &env) override {
        auto args = getArgs(env);
        auto lhs = getArgs(env, Function::lhs);
        args->lst.splice(args->lst.begin(), lhs->lst);
        if (args->lst.size() != 1)
            throw Error("The import function expects exactly one (string) argument.");

        auto filenameArg = args->lst.front();
        auto filename = dynamic_pointer_cast<UserString>(filenameArg);
        if (!filename)
            throw Error("The import function expects a string containing a file name as argument.");

        ifstream file(filename->text);
        if (!file)
            throw Error("Could not open file '" + filename->text + "'.");

        ostringstream file_contents;
        file_contents << file.rdbuf();
        file.close();
        string contents = file_contents.str();

        env.eval(parse(contents));

        return make_shared<List>();
    }
};

struct PrintFunction : Function {
    shared_ptr<Value> apply(Env &env) override {
        auto args = getArgs(env);
        auto lhs = getArgs(env, Function::lhs);
        args->lst.splice(args->lst.begin(), lhs->lst);
        string result;
        if (args->lst.size() == 1) {
            auto val = args->lst.front();
            auto possibleUserString = dynamic_pointer_cast<UserString>(val);
            if (possibleUserString)
                result = toString(possibleUserString->text);
            else
                result = toString(*val);
        } else {
            result = toString(*args);
        }
        cout << result << endl;
        return make_shared<List>();
    }
};

struct ExitFunction : Function {
    shared_ptr<Value> apply(Env &env) override {
        throw ExitNow(0);
    }
};

static void def(Env& env, const string& name, shared_ptr<Function> fn, double precedence, bool specialForm) {
    auto symbol = Symbol::create(name);
    env.assign(symbol, fn);
    fn->symbol = symbol;
    fn->specialForm = specialForm;
    if (precedence != Env::defaultPrecedence)
        dynamic_pointer_cast<SymbolMap>(env.get(Env::ops))->assign(symbol, make_shared<Real>(precedence));
}

template <class T>
static void def(Env* env, const string& name, double precedence = Env::defaultPrecedence, bool specialForm = false) {
    static_assert(std::is_base_of<Function, T>::value, "T is not derived from Function");
    def(*env, name, make_shared<T>(), precedence, specialForm);
}

Env::Env() : outerEnv(nullptr) {
    assign(ops, make_shared<SymbolMap>());

    def<AddFunction>(this, "+", 40);
    def<SubFunction>(this, "-", 39);
    def<MulFunction>(this, "*", 25);
    def<DivFunction>(this, "/", 25);
    def<QuoteFunction>(this, "quote", defaultPrecedence, true);
    def<RevalFunction>(this, "reval", defaultPrecedence, true);
    def<IfFunction>(this, "if", defaultPrecedence, true);
    def<FnDefinition>(this, "fn", defaultPrecedence, true);
    def<SemicolonFunction>(this, ";", 100, true);
    def<AssignFunction>(this, "=", 90, true);
    def<MapFunction>(this, "map", defaultPrecedence, true);
    def<ImportFunction>(this, "import");
    def<PrintFunction>(this, "print");
    def<ExitFunction>(this, "exit");
}
