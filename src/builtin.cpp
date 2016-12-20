#include "common.hpp"

class BuiltinFunction : public Function {
    function<shared_ptr<Value>(shared_ptr<List> args, short pivot)> fn;

public:
    BuiltinFunction(string functionDescription,
                    function<shared_ptr<Value>(shared_ptr<List> args, short pivot)> fn) :
            Function(functionDescription), fn(fn) {}

    shared_ptr<Value> apply(shared_ptr<List> args, short pivot = 0) override {
        return fn(args, pivot);
    }
};

static void define_arithmetic_function(Env &env) {
    env.insert(make_pair(
            "+",
            make_shared<BuiltinFunction>(
                    "add",
                    [](shared_ptr<List> args, short pivot) -> shared_ptr<Value> {
                        Real sum(0.0f);
                        for(auto i = args->lst.begin(); i != args->lst.end(); i++) {
                            shared_ptr<Value> x = *i;

                            if (isType<Real>(*x)) {
                                Real& real = dynamic_cast<Real&>(*x);
                                sum = sum + real;
                            } else {
                                stringstream ss;
                                ss << "The value " << *x << " is not a number.";
                                throw Error(ss.str());
                            }
                        }
                        return make_shared<Real>(sum);
                    }
            )
    ));

    env.insert(make_pair(
            "-",
            make_shared<BuiltinFunction>(
                    "sub",
                    [](shared_ptr<List> args, short pivot) -> shared_ptr<Value> {
                        Real left_sum(0.0f);
                        Real right_sum(0.0f);

                        for(shared_ptr<Value> &x : args->lst) {
                            if (isType<Real>(*x)) {
                                Real& real = dynamic_cast<Real&>(*x);

                                if(pivot--)
                                    left_sum = left_sum + real;
                                else
                                    right_sum = right_sum + real;
                            }
                        }

                        return make_shared<Real>(left_sum -  right_sum);
                    }
            )
    ));

    env.insert(make_pair(
            "*",
            make_shared<BuiltinFunction>(
                    "div",
                    [](shared_ptr<List> args, short pivot) -> shared_ptr<Value> {
                        Real product(1.0f);
                        for(shared_ptr<Value> &x : args->lst) {
                            if (isType<Real>(*x)) {
                                Real& real = dynamic_cast<Real&>(*x);
                                product = product * real;
                            }
                        }
                        return make_shared<Real>(product);
                    }
            )
    ));
}

void define_builtin_functions(Env &env) {
    define_arithmetic_function(env);
}
