#include "common.hpp"

void eval(unique_ptr<Value> expr) {
    cout << *expr << endl;
}
