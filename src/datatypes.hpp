/*
 * Data types
 */

#pragma  once

struct Value {
    Value() = default;
    virtual ostream& display(ostream &o = cout) const = 0;
    virtual ~Value() = default;
    Value(Value const &) = delete;
    void operator=(Value const &) = delete;
};

inline ostream& operator<<(ostream &o, const Value &value) {
    return value.display(o);
}

struct Symbol : public Value {
    const string sym;
    Symbol(const string &s) : sym(s) {}
    ostream& display(ostream &o = cout) const { return o << sym; }
};

struct ListOpen : public Symbol {
    ListOpen() : Symbol("(") {}
};

struct ListClose : public Symbol {
    ListClose() : Symbol(")") {}
};

struct List : public Value {
    list< unique_ptr<Value> > lst;

    ostream& display(ostream &o = cout) const;
};

struct Function : public Value {
    virtual unique_ptr<Value> apply(list< unique_ptr<Value> > args, unsigned short pivot = 0) = 0;
};

struct UserString : public Value {
    const string text;
    UserString(const string &s) : text(s) {}
    ostream& display(ostream &o = cout) const { return o << text; }
};

/*
 * Real is a simple wrapper around a float.
 */
struct Real : public Value {
    Real(double val = nanf("")) : val(val) {}
    Real(const char *str) : val(nanf("")) { fromStr(str); }
    Real(const string &s) : Real(s.c_str()) {}

    inline double operator()() const { return val; }
    inline bool isValid() const { return !::isnan(val); }

    inline Real(const Real& o) : val(o.val) {}
    inline Real& operator=(const Real& o) { val = o.val; return *this; }

    inline Real operator+(const Real &o) const { return val + o.val; }
    inline Real operator-(const Real &o) const { return val - o.val; }
    inline Real operator*(const Real &o) const { return val * o.val; }
    inline Real operator/(const Real &o) const { return val / o.val; }

    inline size_t fromStr(const char *str) {
        char* str_end;
        val = strtod(str, &str_end);
        return str_end - str;
    }

    ostream& display(ostream &o = cout) const;

 private:
    double val;
};

inline ostream& operator<<(ostream &o, const Real &real) {
    return real.display(o);
}
