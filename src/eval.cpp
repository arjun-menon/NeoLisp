#include "common.hpp"

unique_ptr<Value> eval(unique_ptr<Value> v, Env& env) {
    if (isType<Expr>(*v)) {
        return v;
    }
    else if (isType<Symbol>(*v)) {
        //return env[dynamic_cast<Symbol *>(v.get())->sym];
    }
    else if (instanceof<Function>(*v)) {
        //
    }

    return v;
}
