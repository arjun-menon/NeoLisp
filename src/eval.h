#pragma once

typedef map< string, shared_ptr<Value> > Env;

shared_ptr<Value> eval(shared_ptr<Value>, Env&);
