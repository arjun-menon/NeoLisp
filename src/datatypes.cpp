#include "common.hpp"

ostream& Expr::display(ostream &o) const {
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
