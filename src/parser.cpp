#include "common.hpp"

void Parser::parse(List &expr, TokenQueue::iterator& i, int depth) {
    while(i != lexer.tokens.end()) {
        unique_ptr<Value> token = move(*i++);

        if (isType<ListOpen>(*token)) {
            unique_ptr<List> subexpression(new List);
            parse(*subexpression, i, depth + 1);
            expr.lst.emplace_back(move(subexpression));
        }
        else if (isType<ListClose>(*token)) {
            if (depth < 1)
                throw SyntaxError("Missing opening parenthesis.");
            return;
        }
        else {
            expr.lst.emplace_back(move(token));
        }
    }
    if (depth > 0)
        throw SyntaxError("Missing closing parenthesis.");
}

unique_ptr<Value> Parser::parse() {
    TokenQueue::iterator i = lexer.tokens.begin();
    unique_ptr<List> expr(new List);
    parse(*expr, i, 0);
    return expr;
}
