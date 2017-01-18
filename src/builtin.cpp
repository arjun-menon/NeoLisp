#include "common.hpp"

class Builtin {
    struct AddFunction : Function {
        shared_ptr<Value> apply(Env &env, short pivot = 0) override {
            Real sum(0.0f);
            for (auto &k : getArgs(env)->lst) {
                auto x = eval(k, env);

                if (isType<Real>(*x))
                    sum = sum + dynamic_cast<Real &>(*x);
                else
                    errNotNumber(x);
            }
            return make_shared<Real>(sum);
        }
    };

    struct SubFunction : Function {
        shared_ptr<Value> apply(Env &env, short pivot = 0) override {
            Real left_sum(0.0f);
            Real right_sum(0.0f);

            for (auto &k : getArgs(env)->lst) {
                auto x = eval(k, env);

                if (isType<Real>(*x)) {
                    Real &val = dynamic_cast<Real &>(*x);

                    if (pivot-- > 0)
                        left_sum = left_sum + val;
                    else
                        right_sum = right_sum + val;
                } else
                    errNotNumber(x);
            }

            return make_shared<Real>(left_sum - right_sum);
        }
    };

    struct MulFunction : Function {
        shared_ptr<Value> apply(Env &env, short pivot = 0) override {
            Real product(1.0f);
            for (auto &k : getArgs(env)->lst) {
                auto x = eval(k, env);

                if (isType<Real>(*x))
                    product = product * dynamic_cast<Real &>(*x);
                else
                    errNotNumber(x);
            }
            return make_shared<Real>(product);
        }
    };

    struct IfFunction : Function {
        shared_ptr<Value> apply(Env &env, short pivot = 0) override {
            auto args = getArgs(env)->lst;

            if (args.size() != 3)
                throw Error("The ? function expects exactly 3 arguments: cond, conseq, alt.");

            auto cond = args.front();
            args.pop_front();

            auto conseq = args.front();
            args.pop_front();

            auto alt = args.front();
            args.pop_front();

            auto cond_result = eval(cond, env);

            if (!isType<Real>(*cond_result))
                errNotNumber(cond_result);

            auto result_bool = dynamic_cast<Real &>(*cond_result)() != 0.0;

            if (result_bool)
                return eval(conseq, env);
            else
                return eval(alt, env);
        }
    };

    struct AssignFunction : Function {
        shared_ptr<Value> apply(Env &env, short pivot = 0) override {
            auto args = getArgs(env)->lst;

            if (args.size() != 2)
                throw Error("The = function expects exactly 2 arguments: var, val.");

            auto var_name = args.front();
            args.pop_front();

            auto var_val = args.front();
            args.pop_front();

            if (!isType<Symbol>(*var_name))
                throw Error("The variable name must be a symbol.");

            auto var = dynamic_pointer_cast<Symbol>(var_name);
            auto val = eval(var_val, env);

            Env* outerEnv = env.outerEnv;
            if (outerEnv) {
                outerEnv->assign(var, val);
            }

            return make_shared<Real>();
        }
    };

    struct ExitFunction : Function {
        shared_ptr<Value> apply(Env &env, short pivot = 0) override {
            throw ExitNow(0);
        }
    };

    static shared_ptr<List> getArgs(Env &env) {
        auto argsVal = env.get(Function::argsVar);
        if (!isType<List>(*argsVal))
            throw Error("Function call 'args' is not a list!");
        return dynamic_pointer_cast<List>(argsVal);
    }

    static void errNotNumber(shared_ptr<Value> &x) {
        stringstream errMsg;
        errMsg << "The value " << x << " is not a number.";
        throw Error(errMsg.str());
    }

    void define_function(string name, shared_ptr<Function> fn) {
        auto symbol = Symbol::create(name);
        env.assign(symbol, fn);
        fn->symbol = symbol;
    }

    Env &env;

public:
    Builtin(Env &env) : env(env) {}

    void define() {
        define_function("+", make_shared<AddFunction>());
        define_function("-", make_shared<SubFunction>());
        define_function("*", make_shared<MulFunction>());
        define_function("?", make_shared<IfFunction>());
        define_function("=", make_shared<AssignFunction>());
        define_function("q", make_shared<ExitFunction>());
    }
};

void define_builtin(Env &env) {
    Builtin(env).define();
}
