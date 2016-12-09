/*
 * Data types
 */

#pragma  once

class Value
{
 public:
    Value() = default;
    virtual ostream& display(ostream &o = cout) const = 0;
    virtual ~Value() = default;

 protected:
    Value(Value const &) = delete;
    void operator=(Value const &) = delete;
};

class Symbol : public Value
{
    string sym;

public:
    Symbol(const string &s) : sym(s) {}

    inline ostream& display(ostream &o = cout) const { return o << sym; }
};

class List : public Value
{
    
};

class UserString : public Value
{
    const string text;

public:
    UserString(const string &s) : text(s) {}

    inline ostream& display(ostream &o = cout) const { return o << text; }
};

/*
 * Real is a simple wrapper around a float.
 */
class Real : public Value {
    double val;

 public:
    Real(double val = nanf("")) : val(val) {}

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
};

inline ostream& operator<<(ostream &o, const Real &real) {
    return real.display(o);
}
