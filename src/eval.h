#pragma once

typedef map< string, unique_ptr<Value> > Env;

void define_builtin_functions(Env&);

unique_ptr<Value> eval(unique_ptr<Value>, Env&);
