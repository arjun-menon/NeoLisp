#pragma once

typedef map<shared_ptr<Symbol>, shared_ptr<Value>> Env;

shared_ptr<Value> eval(shared_ptr<Value>, Env&);
