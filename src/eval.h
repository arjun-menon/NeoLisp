#pragma once

struct Env : SymbolMap
{
    Env* const outerEnv;
    explicit Env();  // defined in builtin.cpp
    explicit Env(Env& outerEnv) : outerEnv(&outerEnv) {}
    Env& operator=(const Env&) = delete;
    Env(const Env&) = delete;

    inline bool check(shared_ptr<Symbol> symbol) {
        return entries.find(symbol) != entries.end() || (outerEnv != nullptr ? outerEnv->check(symbol) : false);
    }

    static shared_ptr<Symbol> ops;
    static double defaultPrecedence;
    double getPrecedence(shared_ptr<Function> fn);

    shared_ptr<Value> get(shared_ptr<Symbol> symbol);
};

shared_ptr<Value> eval(shared_ptr<Value>, Env&, bool reified=false);
