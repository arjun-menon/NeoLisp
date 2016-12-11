/*
 * Common universal header file
 *
 * For simplicity's sake, just include this header everywhere.
 */

#pragma once

/*
 * C++ Standard Library headers
 */

#include <functional>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <numeric>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <cctype>
#include <memory>
#include <string>
#include <vector>
#include <deque>
#include <cmath>
#include <list>

using namespace std;

/*
 * Common classes, utility templates, and macros.
 */

template<typename T, typename K>
inline bool isType(const K &k) {
    return typeid(T).hash_code() == typeid(k).hash_code();
}

template<typename T>
struct Counter {
    T i = 0;
    Counter(T i) : i(i) {}
    T operator()() {
        return i++;
    }
};

template<typename T>
vector<T> generateSequence(size_t count, size_t start = 0) {
    vector<T> column_indexes(count);
    generate_n(column_indexes.begin(), count, Counter<T>(start));
    return column_indexes;
}

template <typename T>
string toString(const T& obj) {
    ostringstream oss;
    oss << obj;
    return oss.str();
}

class SyntaxError : public exception
{
    // Represents user-caused syntactic (parsing) errors.
private:
    const string error_description;
public:
    virtual const char* what() const throw() { return error_description.c_str(); }
    SyntaxError(string err) : error_description((string)("Error: ")+err) {}
    ~SyntaxError() throw() {}
};

class FatalError : public exception
{
    // These are unrecoverable internal errors, such as assertion failures.
private:
    const string error_description;
public:
    virtual const char* what() const throw() { return error_description.c_str(); }
    FatalError(string err) : error_description((string)("Fatal Error: ")+err) {}
    ~FatalError() throw() {}
};

#define ASSERT(X) if(!(X)) throw fatalError\
((string)("Assertion failure ( ")+(string)(#X)+(string)(" )"))

/*
 * Project header files
 */

#include "datatypes.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "column.hpp"
#include "data_reader.hpp"
#include "table.hpp"
