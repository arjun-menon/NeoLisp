
#pragma once

class Parser
{
    Lexer lexer;

    void parse(Expr &expr, TokenQueue::iterator& i, int depth);

public:
    Parser() = default;
    Parser(const char *str) { parse(str); }
    Parser(const string &s) { parse(s); }

    unique_ptr<Expr> parse();

    unique_ptr<Expr> parse(const char *str) {
        lexer.tokens.clear();
        lexer.lex(str);
        return parse();
    }

    inline unique_ptr<Expr> parse(const string &s) { return parse(s.c_str()); }
};
