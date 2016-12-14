#pragma once

typedef map< string, unique_ptr<Value> > Env;

unique_ptr<Value> eval(unique_ptr<Value>, Env&);
