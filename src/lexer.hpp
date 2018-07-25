#pragma once

typedef deque< shared_ptr<Value> > TokenQueue;

TokenQueue lex(const char *);

ostream& display(const TokenQueue &tokens, ostream &o = cout, bool pretty_print = true);
ostream& operator<<(ostream& stream, const TokenQueue &tokens);
