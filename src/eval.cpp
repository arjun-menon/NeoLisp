#include "common.hpp"

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

        return unique_ptr<Value>(move(evaluatedList));
    }
    else if (isType<Symbol>(*v)) {
        const string &sym = dynamic_cast<Symbol *>(v.get())->sym;
        try {
            return move(env.at(sym));
        } catch(out_of_range) {
            string err_msg = "The symbol '" + sym + "' is not defined.";
            throw Error(err_msg);
        }
    }

    return v;
}
