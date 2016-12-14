#include "common.hpp"

class BuiltinFunction : public Function {
    const string desc;
    function<unique_ptr<Value>(unique_ptr<List> args, unsigned short pivot)> fn;

public:
    BuiltinFunction(string desc,
                    function<unique_ptr<Value>(unique_ptr<List> args, unsigned short pivot)> fn) :
            desc(desc), fn(fn) {}

    unique_ptr<Value> apply(unique_ptr<List> args, unsigned short pivot = 0) override {
        return fn(move(args), pivot);
    }

    ostream& display(ostream &o = cout) const override { return o << "function<" << desc << ">"; }
};

void define_builtin_functions(Env &env) {

    BuiltinFunction add("add",
                        [](unique_ptr<List> args, unsigned short pivot) -> unique_ptr<Value> {
                            return unique_ptr<Value>(new Real(777));
                        });

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
}
