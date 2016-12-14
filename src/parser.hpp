
#pragma once

class Parser
{
    Lexer lexer;
    void parse(Expr &expr, TokenQueue::iterator& i, int depth);
    unique_ptr<Value> parse();

public:
    static unique_ptr<Value> parse(const char *str) {
        Parser parser;
        parser.lexer.lex(str);
        return parser.parse();
    }

    inline static unique_ptr<Value> parse(const string &s) {
        return parse(s.c_str());
    }
};
