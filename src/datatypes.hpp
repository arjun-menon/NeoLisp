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
    inline ostream& display(ostream &o = cout) const { return o << sym; }
};

struct ExprStart : public Symbol {
    ExprStart() : Symbol("(") {}
};

struct ExprEnd : public Symbol {
    ExprEnd() : Symbol(")") {}
};

struct Expr : public Value {
    list< unique_ptr<Value> > lst;

    ostream& display(ostream &o = cout) const {
        o << '(';
        for(auto &value : lst)
            o << *value << ' ';
        o << ')';
        return o;
    }
};

struct UserString : public Value {
    const string text;
    UserString(const string &s) : text(s) {}
    inline ostream& display(ostream &o = cout) const { return o << text; }
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

    inline ostream& display(ostream &o = cout) const {
        if (!isValid())
            o << "NaN";
        else
            o << val;
        return o;
    }

 private:
    double val;
};

inline ostream& operator<<(ostream &o, const Real &real) {
    return real.display(o);
}
