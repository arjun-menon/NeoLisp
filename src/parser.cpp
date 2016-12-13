#include "common.hpp"

static void parse(Expr &expr, TokenQueue& tokens, int depth) {
    while(!tokens.empty()) {
        unique_ptr<Value> tok = move(tokens.front());
        tokens.pop_front();
        expr.lst.emplace_back(move(tok));
    }
}

unique_ptr<Expr> Parser::parse(TokenQueue& tokens) {
    unique_ptr<Expr> expr(new Expr);
    ::parse(*expr, tokens, 0);
    return expr;
}
