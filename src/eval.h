#pragma once

struct Matter
{
    llvm::LLVMContext llvmContext;
    llvm::IRBuilder<> llvmIrBuilder;
    unique_ptr<llvm::Module> llvmModule;
    map<string, llvm::Value *> llvmNamedValues;

    explicit Matter(const string &name): llvmIrBuilder(llvmContext) {
        llvmModule = llvm::make_unique<llvm::Module>(name, llvmContext);
    }
};

struct Env : SymbolMap
{
    Matter& m;
    Env* const outerEnv;
    explicit Env(Matter &_m); // defined in builtin.cpp
    explicit Env(Env& _outerEnv) : m(_outerEnv.m), outerEnv(&_outerEnv) {}
    Env& operator=(const Env&) = delete;
    Env(const Env&) = delete;

    inline bool check(shared_ptr<Symbol> symbol) {
        return entries.find(symbol) != entries.end() || (outerEnv != nullptr ? outerEnv->check(symbol) : false);
    }

    static shared_ptr<Symbol> ops;
    static double defaultPrecedence;
    double getPrecedence(shared_ptr<Function> fn);

    shared_ptr<Value> get(shared_ptr<Symbol> symbol);

    shared_ptr<Value> eval(shared_ptr<Value> v, bool reified=false);
};
