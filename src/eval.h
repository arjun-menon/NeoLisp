#pragma once

typedef map<shared_ptr<Symbol>, shared_ptr<Value>, shared_ptr_comparator<Symbol>> Env;

shared_ptr<Value> eval(shared_ptr<Value>, Env&);
