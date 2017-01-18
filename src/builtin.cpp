#include "common.hpp"

class Builtin {
    struct AddFunction : Function {
        shared_ptr<Value> apply(shared_ptr<List> args, short pivot = 0) override {
            Real sum(0.0f);
            for (shared_ptr<Value> &x : args->lst) {
                if (isType<Real>(*x))
                    sum = sum + dynamic_cast<Real &>(*x);
                else
                    errNotNumber(x);
            }
            return make_shared<Real>(sum);
        }
    };

    struct SubFunction : Function {
        shared_ptr<Value> apply(shared_ptr<List> args, short pivot = 0) override {
            Real left_sum(0.0f);
            Real right_sum(0.0f);

            for (shared_ptr<Value> &x : args->lst) {
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
        shared_ptr<Value> apply(shared_ptr<List> args, short pivot = 0) override {
            Real product(1.0f);
            for (shared_ptr<Value> &x : args->lst) {
                if (isType<Real>(*x))
                    product = product * dynamic_cast<Real &>(*x);
                else
                    errNotNumber(x);
            }
            return make_shared<Real>(product);
        }
    };

    struct ExitFunction : Function {
        shared_ptr<Value> apply(shared_ptr<List> args, short pivot = 0) override {
            throw ExitNow(0);
        }
    };

    static void errNotNumber(shared_ptr<Value> x) {
        stringstream errMsg;
        errMsg << "The value " << *x << " is not a number.";
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
        define_function("q", make_shared<ExitFunction>());
    }
};

void define_builtin(Env &env) {
    Builtin(env).define();
}
