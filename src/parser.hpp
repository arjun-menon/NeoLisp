
#pragma once

class Parser
{
    Lexer lexer;

public:
    Parser() = default;
    Parser(const char *str) { parse(str); }
    Parser(const string &s) { parse(s); }

    unique_ptr<Expr> parse(TokenQueue& tokens);

    inline unique_ptr<Expr> parse(const char *str) {
        lexer.tokens.clear();
        return parse(lexer.lex(str));
    }

    inline unique_ptr<Expr> parse(const string &s) { return parse(s.c_str()); }
};
