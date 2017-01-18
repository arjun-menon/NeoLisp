#include "common.hpp"

shared_ptr<Value> Env::get(shared_ptr<Symbol> symbol) {
    if(variables.find(symbol) != variables.end()) {
        return variables.at(symbol);
    }
    else {
        if (outerEnv) {
            return outerEnv->get(symbol);
        } else {
            string err_msg = "The symbol '" + toString(*symbol) + "' is not defined.";
            throw Error(err_msg);
        }
    }
}

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

            Env fnEnv(&env);
            fnEnv.assign(Function::argsVar, evaluatedList);

            return fn.apply(fnEnv);
        }

        return shared_ptr<Value>(evaluatedList);
    }
    else if (isType<Symbol>(*v)) {
        return env.get( dynamic_pointer_cast<Symbol>(v) );
    }

    return v;
}
