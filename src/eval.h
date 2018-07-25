#pragma once

class Env
{
    map<shared_ptr<Symbol>, shared_ptr<Value>, shared_ptr_comparator<Symbol>> variables;

public:
    Env* const outerEnv;
    explicit Env();  // defined in builtin.cpp
    explicit Env(Env& outerEnv) : outerEnv(&outerEnv) {}

    inline void assign(shared_ptr<Symbol> symbol, shared_ptr<Value> value) {
        variables[symbol] = value;
    }

    inline bool check(shared_ptr<Symbol> symbol) {
        return variables.find(symbol) != variables.end() || (outerEnv != nullptr ? outerEnv->check(symbol) : false);
    }

    shared_ptr<Value> get(shared_ptr<Symbol> symbol);
};

shared_ptr<Value> eval(shared_ptr<Value>, Env&, bool reified=false);
