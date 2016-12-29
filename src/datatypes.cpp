#include "common.hpp"

shared_ptr<Symbol> Symbol::create(const string &s)
{
    //return make_shared<Symbol>(s);
    return std::shared_ptr<Symbol>(new Symbol(s));
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

ostream& Real::display(ostream &o) const {
    if (isValid())
        o << val;
    else
        o << "NaN";
    return o;
}
