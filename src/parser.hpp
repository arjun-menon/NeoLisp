#pragma once

shared_ptr<Value> parse(TokenQueue&);

inline shared_ptr<Value> parse(const char *str) {
    Lexer lexer;
    return parse(lexer.lex(str));
}

inline static shared_ptr<Value> parse(const string &s) { return parse(s.c_str()); }
