#include "common.hpp"

static void parse(TokenQueue& tokens, List &expr, TokenQueue::iterator& i, int depth) {
    while(i != tokens.end()) {
        shared_ptr<Value> token = *i++;

        if (token == List::open) {
            auto subexpression = make_shared<List>();
            parse(tokens, *subexpression, i, depth + 1);
            expr.lst.emplace_back(subexpression);
        }
        else if (token == List::close) {
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

shared_ptr<Value> parse(TokenQueue& tokens) {
    auto expr = make_shared<List>();
    TokenQueue::iterator i = tokens.begin();
    parse(tokens, *expr, i, 0);
    return shared_ptr<Value>(expr);
}
