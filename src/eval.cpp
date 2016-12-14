#include "common.hpp"

unique_ptr<Value> eval(unique_ptr<Value> v) {
    if (isType<Expr>(*v)) {
        return v;
    }

    return v;
}
