#pragma once

shared_ptr<Value> parse(TokenQueue);

inline shared_ptr<Value> parse(const char *str) { return parse(lex(str)); }
inline shared_ptr<Value> parse(const string &s) { return parse(s.c_str()); }
