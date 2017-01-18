#include "common.hpp"

map<string, weak_ptr<Symbol>> Symbol::existingSymbols;

shared_ptr<Symbol> Symbol::create(const string &name)
{
    if(existingSymbols.find(name) != existingSymbols.end())
        if(auto symbol = existingSymbols[name].lock())
            return symbol;

    auto symbol = shared_ptr<Symbol>(new Symbol(name));
    existingSymbols[name] = symbol;
    return symbol;
}

shared_ptr<Symbol> List::open = Symbol::create("(");
shared_ptr<Symbol> List::close = Symbol::create(")");

ostream& List::display(ostream &o) const {
    o << '(';
    for(auto i = lst.begin() ;; i++) {
        if (i == lst.end()) {
            o << ')';
            break;
        }
        if (i != lst.begin()) {
            o << ' ';
        }
        o << **i;
    }
    return o;
}

ostream& Function::display(ostream &o) const {
    o << "function<";
    if(auto s = symbol.lock())
        s->display(o);
    else
        o << "lambda";
    return o << ">";
}

ostream& Real::display(ostream &o) const {
    if (isValid())
        o << val;
    else
        o << "NaN";
    return o;
}
