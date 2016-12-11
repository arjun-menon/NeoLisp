
#pragma once

class Lexer
{
    string token;

    void addToken();

public:
    deque<string> tokens;

    void lex(const char *sp);
    ostream& display(ostream &o = cout, bool pretty_print = true) const;
};
