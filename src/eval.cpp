#include "common.hpp"

unique_ptr<Value> eval(unique_ptr<Value> v, Env& env) {
    if (isType<List>(*v)) {
        /*
         * Evaluate all of the elements in the list
         */
        List* list = dynamic_cast<List*>(v.get());
        unique_ptr<List> evaluatedList(new List);

        for(auto i =  list->lst.begin(); i != list->lst.end(); i++)
            evaluatedList->lst.emplace_back(eval(move(*i), env));

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
    else if (instanceof<Function>(*v)) {
        //
    }

    return v;
}
