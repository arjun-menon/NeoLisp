
#pragma once

typedef deque< unique_ptr<Value> > TokenQueue;

class Lexer
{
    string token;

    void constructValue();
    void addToken();

public:
    TokenQueue tokens;

    static ostream& display(const TokenQueue &tokens, ostream &o = cout, bool pretty_print = true);
    ostream& display(ostream &o = cout, bool pretty_print = true) { return display(tokens, o, pretty_print); }

    void lex(const char *sp);
};

ostream& operator<<(ostream& stream, const TokenQueue &tokens);
