#pragma once

class Env : private SymbolMap
{
public:
    Env* const outerEnv;
    explicit Env();  // defined in builtin.cpp
    explicit Env(Env& outerEnv) : outerEnv(&outerEnv) {}
    Env& operator=(const Env&) = delete;
    Env(const Env&) = delete;

    inline void assign(shared_ptr<Symbol> symbol, shared_ptr<Value> value) {
        entries[symbol] = value;
    }

    inline bool check(shared_ptr<Symbol> symbol) {
        return entries.find(symbol) != entries.end() || (outerEnv != nullptr ? outerEnv->check(symbol) : false);
    }

    shared_ptr<Value> get(shared_ptr<Symbol> symbol);
};

shared_ptr<Value> eval(shared_ptr<Value>, Env&, bool reified=false);
