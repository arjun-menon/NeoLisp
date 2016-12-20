
#pragma once

typedef deque< shared_ptr<Value> > TokenQueue;

class Lexer
{
    string token;

    void constructValue();
    void addToken();

public:
    TokenQueue tokens;

    static ostream& display(const TokenQueue &tokens, ostream &o = cout, bool pretty_print = true);
    ostream& display(ostream &o = cout, bool pretty_print = true) const { return display(tokens, o, pretty_print); }

    TokenQueue& lex(const char *sp);
    TokenQueue& lex(const string &s) { return lex(s.c_str()); }
};

ostream& operator<<(ostream& stream, const TokenQueue &tokens);
