#include "common.hpp"

shared_ptr<Value> Env::get(shared_ptr<Symbol> symbol) {
    if(entries.find(symbol) != entries.end()) {
        return entries.at(symbol);
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

shared_ptr<Symbol> Env::ops = Symbol::create("ops");
double Env::defaultPrecedence = 0;

double Env::getPrecedence(shared_ptr<Function> fn) {
    auto ops = dynamic_pointer_cast<SymbolMap>(get(Env::ops));
    if(ops && !fn->symbol.expired()) {
        auto name = fn->symbol.lock();
        auto op = ops->entries.find(name);
        if (op != ops->entries.end()) {
            auto op_precedence = op->second;
            if (instanceof<Real>(op_precedence))
                return (*dynamic_pointer_cast<Real>(op_precedence))();
        }
    }
    return Env::defaultPrecedence;
}

shared_ptr<Value> Env::eval(shared_ptr<Value> v, bool reified) {
    if (instanceof<List>(v)) {
        auto vList = dynamic_pointer_cast<List>(v);

        if (vList->lst.empty()) {
            return vList;
        }

        shared_ptr<Function> fn;
        auto pos = vList->lst.end();
        for (auto it = vList->lst.begin(); it != vList->lst.end(); it++) {
            shared_ptr<Function> possibleFn = dynamic_pointer_cast<Function>(*it);
            if (!possibleFn && instanceof<Symbol>(*it)) {
                auto symbol = dynamic_pointer_cast<Symbol>(*it);
                if (check(symbol)) {
                    possibleFn = dynamic_pointer_cast<Function>(get(symbol));
                }
            }
            if (possibleFn && (!fn || getPrecedence(fn) < getPrecedence(possibleFn))) {
                fn = possibleFn;
                pos = it;
            }
        }

        if (pos != vList->lst.end()) { // there's a function in vList
            auto lhs = make_shared<List>(vList->lst.begin(), pos);
            auto rhs = make_shared<List>(++pos, vList->lst.end());

            Env fnEnv(*this);
            if (fn->specialForm) {
                fnEnv.assign(Function::args, rhs);
                fnEnv.assign(Function::lhs, lhs);
            } else {
                fnEnv.assign(Function::args, eval(rhs));
                fnEnv.assign(Function::lhs, eval(lhs));
            }
            return fn->apply(fnEnv);
        } else if (!reified) { // no function present in vList
            auto evaluatedList = make_shared<List>();
            transform(vList->lst.begin(), vList->lst.end(), std::back_inserter(evaluatedList->lst),
                      [this](shared_ptr<Value> elem) -> shared_ptr<Value> { return eval(elem); });
            return eval(evaluatedList, true);
        }
    }
    else if (instanceof<Symbol>(v)) {
        return get( dynamic_pointer_cast<Symbol>(v) );
    }

    return v;
}
