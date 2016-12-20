#include "common.hpp"

void Parser::parse(List &expr, TokenQueue::iterator& i, int depth) {
    while(i != lexer.tokens.end()) {
        shared_ptr<Value> token = *i++;

        if (isType<ListOpen>(*token)) {
            auto subexpression = make_shared<List>();
            parse(*subexpression, i, depth + 1);
            expr.lst.emplace_back(subexpression);
        }
        else if (isType<ListClose>(*token)) {
            if (depth < 1)
                throw Error("Missing opening parenthesis.");
            return;
        }
        else {
            expr.lst.emplace_back(token);
        }
    }
    if (depth > 0)
        throw Error("Missing closing parenthesis.");
}

shared_ptr<Value> Parser::parse() {
    TokenQueue::iterator i = lexer.tokens.begin();
    auto expr = make_shared<List>();
    parse(*expr, i, 0);
    return shared_ptr<Value>(expr);
}
