/*
 * A cell in the table is represented by the `Cell` class here.
 */

#ifndef VECTORCALC_CELL_H
#define VECTORCALC_CELL_H

typedef double real;

inline real real_nan() { return nan(""); }

inline size_t str_to_real(const char *str, real &val) {
    char* str_end;
    val = strtod(str, &str_end);
    return str_end - str;
}

/*
 * Cell stores a real number (double).
 */
class Cell {
    real val;

public:
    Cell() : val(real_nan()) {}
    Cell(real val) : val(val) {}

    inline real get() const { return val; }
    inline bool isPresent() const { return !isnan(val); }
    inline Cell& operator=(const Cell& o) { val = o.val; return *this; }

    inline Cell operator+(const Cell &o) const { return val + o.val; };
    inline Cell operator-(const Cell &o) const { return val - o.val; };
    inline Cell operator*(const Cell &o) const { return val * o.val; };
    inline Cell operator/(const Cell &o) const { return val / o.val; };
};

inline ostream& operator<<(ostream &stream, const Cell &cell) {
    if (!cell.isPresent())
        stream << "NULL";
    else
        stream << cell.get();
    return stream;
}

#endif //VECTORCALC_CELL_H
