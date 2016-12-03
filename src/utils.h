
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
