NeoLisp [![](https://travis-ci.org/arjun-menon/NeoLisp.svg?branch=master)](https://travis-ci.org/arjun-menon/NeoLisp)
=======

NeoLisp is a LISP-like language that I'm building to experiment
and toy around with a few new language ideas.

I'll expand this readme, and document the language thoroughly
once it's a bit more mature. In the meanwhile, you can find
NeoLisp code samples/snippets in `unit_tests/language_tests.cpp`.

#### Infix Expressions

One interesting aspect of NeoLisp is its function application rules.
In addition to supporting LISP-style S-expression function application,
it has a novel set of function application that allow traditional
*precedence-based infix expressions*.

Functions in NeoLisp can have a floating-point precedence value
associated with them. When NeoLisp evaluates a list, it finds the
function with the lowest precedence, and applies the list against
this function, exposing in the items on the left as `lhs`, and the
ones on the right as `args`.

##### The Semicolon

NeoLisp doesn't have a `progn` (Common LISP) or `begin` (Scheme) special form.
Instead, it has `;`, an equivalent that is internally implemented as a
delayed-evaluation function of zero precedence. For example, to evaluate
`exprA ; exprB`, the `;` function first evaluates `exprA` and binds its
result to `_`, then evaluates `exprB` and returns it as the result
of the `;` function. This allows `exprA ; exprB ; exprC ; exprD` to
be executed in the implied order.
