#include "common.hpp"

class BuiltinFunction : public Function {
    function<unique_ptr<Value>(unique_ptr<List> args, unsigned short pivot)> fn;

public:
    BuiltinFunction(string functionDescription,
                    function<unique_ptr<Value>(unique_ptr<List> args, unsigned short pivot)> fn) :
            Function(functionDescription), fn(fn) {}

    unique_ptr<Value> apply(unique_ptr<List> args, unsigned short pivot = 0) override {
        return fn(move(args), pivot);
    }
};

static void define_arithmetic_function(Env &env) {
    env.insert(make_pair(
            "+",
            unique_ptr<Value>(new BuiltinFunction(
                    "add",
                    [](unique_ptr<List> args, unsigned short pivot) -> unique_ptr<Value> {
                        unique_ptr<Real> sum(new Real(0.0f));
                        for(unique_ptr<Value> &x : args->lst) {
                            if (isType<Real>(*x)) {
                                Real& real = dynamic_cast<Real&>(*x);
                                *sum = *sum + real;
                            }
                        }
                        return sum;
                    })
            )
    ));

    env.insert(make_pair(
            "-",
            unique_ptr<Value>(new BuiltinFunction(
                    "sub",
                    [](unique_ptr<List> args, unsigned short pivot) -> unique_ptr<Value> {
                        Real left_sum(0.0f);
                        Real right_sum(0.0f);

                        for(unique_ptr<Value> &x : args->lst) {
                            if (isType<Real>(*x)) {
                                Real& real = dynamic_cast<Real&>(*x);

                                if(pivot--)
                                    left_sum = left_sum + real;
                                else
                                    right_sum = right_sum + real;
                            }
                        }

                        return unique_ptr<Real>(new Real(left_sum -  right_sum));
                    })
            )
    ));

    env.insert(make_pair(
            "*",
            unique_ptr<Value>(new BuiltinFunction(
                    "div",
                    [](unique_ptr<List> args, unsigned short pivot) -> unique_ptr<Value> {
                        unique_ptr<Real> product(new Real(1.0f));
                        for(unique_ptr<Value> &x : args->lst) {
                            if (isType<Real>(*x)) {
                                Real& real = dynamic_cast<Real&>(*x);
                                *product = *product * real;
                            }
                        }
                        return product;
                    })
            )
    ));
}

void define_builtin_functions(Env &env) {
    define_arithmetic_function(env);
}
