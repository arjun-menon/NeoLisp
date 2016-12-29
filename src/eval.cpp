#include "common.hpp"

shared_ptr<Value> eval(shared_ptr<Value> v, Env& env) {
    if (isType<List>(*v)) {
        /*
         * Evaluate all of the elements in the list
         */
        List& list = dynamic_cast<List&>(*v);
        auto evaluatedList = make_shared<List>();

        for(auto i =  list.lst.begin(); i != list.lst.end(); i++)
            evaluatedList->lst.emplace_back(eval(*i, env));

        /*
         * If the expression is function-applicable, execute it
         */

        if (evaluatedList->lst.size() > 0 &&
                instanceof<Function>(*evaluatedList->lst.front())) {
            shared_ptr<Value> fn_(evaluatedList->lst.front());
            Function& fn = dynamic_cast<Function&>(*fn_);
            evaluatedList->lst.pop_front();
            return fn.apply(evaluatedList);
        }

        return shared_ptr<Value>(evaluatedList);
    }
    else if (isType<Symbol>(*v)) {
        shared_ptr<Symbol> symbol = dynamic_pointer_cast<Symbol>(v);
        try {
            return env.at(symbol);
        } catch(out_of_range) {
            string err_msg = "The symbol '" + toString(*symbol) + "' is not defined.";
            throw Error(err_msg);
        }
    }

    return v;
}
