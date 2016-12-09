
#pragma once

template<class T>
struct Counter {
    T i = 0;
    Counter(T i) : i(i) {}
    T operator()() {
        return i++;
    }
};

template<class T>
vector<T> generateSequence(size_t count, size_t start = 0) {
    vector<T> column_indexes(count);
    generate_n(column_indexes.begin(), count, Counter<T>(start));
    return column_indexes;
}

template <class T>
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

// Macros
#define UI unsigned int
#define UC unsigned char
#define ASSERT(X) if(!(X)) throw fatalError\
((string)("Assertion failure ( ")+(string)(#X)+(string)(" )"))
