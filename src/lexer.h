
#pragma once

class Lexer
{
    string token;

    static void trim(string &str, const string to_remove = " \t");
    void addToken();

public:
    deque<string> tokens;

    void lex(const char *sp);
    string toString(bool pretty_print=false);
};
