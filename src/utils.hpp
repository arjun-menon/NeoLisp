/*
 * Utility classes, templates, and macros.
 */

#pragma once

template< typename T >
struct shared_ptr_comparator {
    bool operator()(const std::shared_ptr<T> &a, const std::shared_ptr<T> &b) const {
        return std::less<T>()(*a, *b);
    }
};

template<typename T, typename K>
inline bool isType(const K &k) {
    return typeid(T).hash_code() == typeid(k).hash_code();
}

template<typename T, typename K>
inline bool instanceof(K &k) {
    return dynamic_cast<T*>(&k) != nullptr;
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

class Error : public exception
{
    // Represents user-caused syntactic or run-time errors.
private:
    const string error_description;
public:
    virtual const char* what() const throw() { return error_description.c_str(); }
    Error(string err) : error_description((string)("Error: ")+err) {}
    ~Error() throw() {}
};

#define ASSERT(X) if(!(X)) throw Error((string)("Assertion failure ( ")+(string)(#X)+(string)(" )"))
