#include "common.hpp"

struct AddFunction : Function {
    AddFunction() : Function("add") {};

    shared_ptr<Value> apply(shared_ptr<List> args, short pivot = 0) override {
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
};

struct SubFunction : Function {
    SubFunction() : Function("sub") {};

    shared_ptr<Value> apply(shared_ptr<List> args, short pivot = 0) override {
        Real left_sum(0.0f);
        Real right_sum(0.0f);

        for(shared_ptr<Value> &x : args->lst) {
            if (isType<Real>(*x)) {
                Real& real = dynamic_cast<Real&>(*x);

                if(pivot-- > 0)
                    left_sum = left_sum + real;
                else
                    right_sum = right_sum + real;
            }
        }

        return make_shared<Real>(left_sum -  right_sum);
    }
};

struct MulFunction : Function {
    MulFunction() : Function("mul") {};

    shared_ptr<Value> apply(shared_ptr<List> args, short pivot = 0) override {
        Real product(1.0f);
        for(shared_ptr<Value> &x : args->lst) {
            if (isType<Real>(*x)) {
                Real& real = dynamic_cast<Real&>(*x);
                product = product * real;
            }
        }
        return make_shared<Real>(product);
    }
};

void define_builtin_functions(Env &env) {
    env.insert(make_pair("+", make_shared<AddFunction>()));
    env.insert(make_pair("-", make_shared<SubFunction>()));
    env.insert(make_pair("*", make_shared<MulFunction>()));
}
