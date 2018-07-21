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
        auto vList = dynamic_pointer_cast<List>(v);

        if (!vList->lst.size()) {
            return vList;
        }

        auto head = eval(vList->lst.front(), env);
        vList->lst.pop_front();

        if(instanceof<Function>(*head)) {
            auto fn = dynamic_pointer_cast<Function>(head);

            Env fnEnv(&env);
            fnEnv.assign(Function::argsVar, vList);

            return fn->apply(fnEnv);
        }
        else {
            vList->lst.push_front(head);
        }
    }
    else if (isType<Symbol>(*v)) {
        return env.get( dynamic_pointer_cast<Symbol>(v) );
    }

    return v;
}
