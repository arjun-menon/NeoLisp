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

shared_ptr<Value> eval(shared_ptr<Value> v, Env& env, bool reified) {
    if (isType<List>(*v)) {
        auto vList = dynamic_pointer_cast<List>(v);

        if (!vList->lst.size()) {
            return vList;
        }

        short i = 0, pivot = 0;
        shared_ptr<Function> fn;
        auto pos = vList->lst.end();
        for (auto it = vList->lst.begin(); it != vList->lst.end(); it++, i++) {
            shared_ptr<Function> possibleFn = dynamic_pointer_cast<Function>(*it);
            if (!possibleFn && isType<Symbol>(**it)) {
                auto symbol = dynamic_pointer_cast<Symbol>(*it);
                if (env.check(symbol)) {
                    possibleFn = dynamic_pointer_cast<Function>(env.get(symbol));
                }
            }
            if (possibleFn && (!fn || fn->precedence > possibleFn->precedence)) {
                fn = possibleFn;
                pivot = i;
                pos = it;
            }
        }

        if (pos != vList->lst.end()) {
            auto val = *pos;
            auto reinsertAt = vList->lst.erase(pos);

            Env fnEnv(env);
            fnEnv.assign(Function::argsVar, vList);
            auto result = fn->apply(fnEnv, pivot);

            vList->lst.insert(reinsertAt, val);
            return result;
        } else if (!reified) {
            auto evaluatedList = make_shared<List>();
            for (auto &elem : vList->lst)
                evaluatedList->lst.push_back(eval(elem, env));
            return eval(evaluatedList, env, true);
        }
    }
    else if (isType<Symbol>(*v)) {
        return env.get( dynamic_pointer_cast<Symbol>(v) );
    }

    return v;
}
