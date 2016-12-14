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

unique_ptr<Value> eval(unique_ptr<Value> v, Env& env) {
    if (isType<List>(*v)) {
        /*
         * Evaluate all of the elements in the list
         */
        List& list = dynamic_cast<List&>(*v);
        unique_ptr<List> evaluatedList(new List);

        for(auto i =  list.lst.begin(); i != list.lst.end(); i++)
            evaluatedList->lst.emplace_back(eval(move(*i), env));

        /*
         * If the expression is function-applicable, execute it
         */

        if (evaluatedList->lst.size() > 0 &&
                instanceof<Function>(*evaluatedList->lst.front())) {
            unique_ptr<Value> fn_(move(evaluatedList->lst.front()));
            Function& fn = dynamic_cast<Function&>(*fn_);
            evaluatedList->lst.pop_front();
            return fn.apply(move(evaluatedList));
        }

        return evaluatedList;
    }
    else if (isType<Symbol>(*v)) {
        const string &sym = dynamic_cast<Symbol *>(v.get())->sym;
        try {
            return move(env.at(sym));
        } catch(out_of_range) {
            string err_msg = "The symbol '" + sym + "' is not defined.";
            throw SyntaxError(err_msg);
        }
    }

    return v;
}
