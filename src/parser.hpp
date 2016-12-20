
#pragma once

class Parser
{
    Lexer lexer;

    Parser() = default;
    void parse(List &expr, TokenQueue::iterator& i, int depth);
    shared_ptr<Value> parse();

public:
    static shared_ptr<Value> parse(const char *str) {
        Parser parser;
        parser.lexer.lex(str);
        return parser.parse();
    }

    inline static shared_ptr<Value> parse(const string &s) {
        return parse(s.c_str());
    }
};
