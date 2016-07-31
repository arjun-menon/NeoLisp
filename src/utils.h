
#ifndef VECTORCALC_UTILS_H
#define VECTORCALC_UTILS_H

template<class T>
struct Counter {
    T i = 0;
    Counter(T i) : i(i) {}
    T operator()() {
        return i++;
    }
};

template<class T>
vector<T> generateSequence(T count, T start = 0) {
    vector<T> column_indexes(count);
    generate_n(column_indexes.begin(), count, Counter<T>(start));
    return column_indexes;
}

#endif //VECTORCALC_UTILS_H
