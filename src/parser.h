
#pragma once

typedef list<tok_chain> frag_chain;
frag_chain Fragmentize(deque<string> &toks);
string display_frags(const frag_chain &, bool linear);
