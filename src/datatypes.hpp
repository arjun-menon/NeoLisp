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

class Env;

struct Symbol : Value {
    static shared_ptr<Symbol> create(const string &name);
    ostream& display(ostream &o) const override { return o << name; }
    inline bool operator<(const Symbol &o) const { return less<string>()(name, o.name); }

private:
    const string name;
    Symbol(const string &name) : name(name) {}
    static map<string, weak_ptr<Symbol>> existingSymbols;
};

struct List : Value {
    list< shared_ptr<Value> > lst;

    ostream& display(ostream &o) const override;

    static shared_ptr<Symbol> open;   // "("
    static shared_ptr<Symbol> close;  // ")"
};

struct Function : Value {
    virtual shared_ptr<Value> apply(Env &env, short pivot = 0) = 0;
    ostream& display(ostream &o) const override;
    weak_ptr<Symbol> symbol;
    static float defaultPrecedence;
    float precedence = defaultPrecedence;
    static shared_ptr<Symbol> argsVar;  // "args"
};

struct UserString : Value {
    const string text;
    UserString(const string &s) : text(s) {}
    ostream& display(ostream &o) const override { return o << "'" << text << "'"; }
};

/*
 * Real is a simple wrapper around a float.
 */
struct Real : Value {
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

    ostream& display(ostream &o) const override;

 private:
    double val;
};

inline ostream& operator<<(ostream &o, const Real &real) {
    return real.display(o);
}
