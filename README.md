NeoLisp [![](https://travis-ci.org/arjun-menon/NeoLisp.svg?branch=master)](https://travis-ci.org/arjun-menon/NeoLisp)
=======

NeoLisp is a LISP-like language that I'm building to experiment
and toy around with a few new language ideas.

I'll expand this readme, and document the language thoroughly
once it's a bit more mature. In the meanwhile, you can find
NeoLisp code samples/snippets in [`unit_tests/language_tests.cpp`](https://github.com/arjun-menon/NeoLisp/blob/master/unit_tests/language_tests.cpp).

## Language Notes

#### Infix Expressions

One interesting aspect of NeoLisp is its function application rules.
In addition to supporting LISP-style S-expression function application,
it has a novel approach to function application that allows traditional
*precedence-based infix expressions*.

Functions in NeoLisp can have a floating-point precedence value
associated with them. When NeoLisp evaluates a list, it finds the
function with the lowest precedence, and applies the list against
this function, passing in the items on the left as `lhs`, and the
ones on the right as `args`. Traditional LISP-style function invokation
in the form of `(+ 1 2)` also works. The arguments on the right are
simply passed as `args` to the function. As a result, both `(+ 1 2)`
and `1 + 2` are valid and equivalent. Differing precedence operators
means that an expression like `1 + 2 * 3 + 4` is evaluated in the
expected. This novel approach to function application means that it's
trivially easy to define new binary operators in a NeoLisp program.

##### The Semicolon

NeoLisp doesn't have a `progn` (Common LISP) or `begin` (Scheme) special form.
Instead, it has `;`, an equivalent that is internally implemented as a
delayed-evaluation function of zero precedence. For example, to evaluate
`exprA ; exprB`, the `;` function first evaluates `exprA` and binds its
result to `_`, then evaluates `exprB` and returns its result as the result
of the `;` function. This allows `exprA ; exprB ; exprC ; exprD` to
be executed in the implied order.

## Building & Running

The script `build.sh` both builds and tests this project. The exit code of
`build.sh` indicates whether unit tests passed.
You need CMake, and a recent version of GCC or Clang that supports C++17.
The unit tests are written using [Catch2](https://github.com/catchorg/Catch2),
and so you'll need to make sure the Catch header file `catch.hpp` is accessible
from the include path, in order to be able to run unit tests.
I use Valgrind to test for memory leaks and `cppcheck` for additional
static code quality checks — they're entirely optional.

I strongly recommend using [`rlwrap`](https://github.com/hanslub42/rlwrap)
when using the NeoLisp REPL for a better exeprience. You can configure it
to match parentheses by setting `set blink-matching-paren on` in your
[`.inputrc`](https://www.topbug.net/blog/2017/07/31/inputrc-for-humans/) (readline configuration file).

To re-iterate, the dependencies are:

 * [CMake](https://cmake.org/) 3.5+
 * [GCC](https://gcc.gnu.org/) 9+ or [Clang](https://clang.llvm.org/) 8+
 * [LLVM](https://llvm.org/) Core 8+ (the development libraries)
 * [catch.hpp](https://github.com/catchorg/Catch2) (optional, for testing)
 * [Cppcheck](https://en.wikipedia.org/wiki/Cppcheck) (optional, for testing)
 * [Valgrind](https://en.wikipedia.org/wiki/Valgrind) (optional, for testing)
 * [rlwrap](https://github.com/hanslub42/rlwrap) (optional, for a better REPL experience)

After building, you can run the REPL with `rlwrap ./NeoLisp`.

### Work In Progress

I'm currently working on getting NeoLisp to a point where it **compiles to machine code via LLVM**.

## Credit & License

Many thanks to Peter Norvig's Lispy guides ([1](https://norvig.com/lispy.html), [2](http://norvig.com/lispy2.html)) and the [LLVM Tutorial](https://llvm.org/docs/tutorial/).

The code herein is made available under the [Apache License 2.0](https://www.apache.org/licenses/LICENSE-2.0).
