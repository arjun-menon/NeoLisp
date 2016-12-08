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

/*
 * Real is a simple wrapper around a float.
 */
class Real {
    float val;

 public:
    Real(float val = nanf("")) : val(val) {}

    inline float operator()() const { return val; }
    inline bool isValid() const { return !::isnan(val); }

    inline Real(const Real& o) : val(o.val) {}
    inline Real& operator=(const Real& o) { val = o.val; return *this; }

    inline Real operator+(const Real &o) const { return val + o.val; }
    inline Real operator-(const Real &o) const { return val - o.val; }
    inline Real operator*(const Real &o) const { return val * o.val; }
    inline Real operator/(const Real &o) const { return val / o.val; }

    inline size_t fromStr(const char *str) {
        char* str_end;
        val = strtof(str, &str_end);
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

class Number : public Value
{
 public:
    const Real val;
    Number(Real val) : val(val) {}
};
