
#pragma once

class Lexer
{
    string token;

    void constructValue();
    void addToken();

public:
    deque< unique_ptr<Value> > tokens;

    void lex(const char *sp);
    ostream& display(ostream &o = cout, bool pretty_print = true) const;
};
