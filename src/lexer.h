
#pragma once

typedef deque<string> tok_chain; // NOTE !!! tok_chain = fragment

tok_chain Tokenize(const char *sp);
string display_toks(const tok_chain &, bool linear);
