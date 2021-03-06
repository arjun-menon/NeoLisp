#include <catch.hpp>
#include "../src/common.hpp"

using Catch::Matchers::Equals;
using Catch::Matchers::Contains;

static string eval(const char *inputExpression) {
    Matter m("NeoLisp-tests");
    Env env(m);
    env.eval(parse("import 'std.neolisp'"));
    return toString(*env.eval(parse(inputExpression)));
}

TEST_CASE("Eval basic arithmetic expressions") {
    CHECK_THAT(eval(""), Equals("()"));

    CHECK_THAT(eval("+ 1 2 3"), Equals("6"));
    CHECK_THAT(eval("* 4 5"), Equals("20"));
    CHECK_THAT(eval("+ 1 2 (* 4 5) 3 4"), Equals("30"));

    CHECK_THAT(eval("3 + 5"), Equals("8"));
    CHECK_THAT(eval("7 * 5"), Equals("35"));
    CHECK_THAT(eval("7 - 5"), Equals("2"));
    CHECK_THAT(eval("80 / 20"), Equals("4"));
    CHECK_THAT(eval("(4 * 20) / (11 + 9)"), Equals("4"));
    CHECK_THAT(eval("(7 + (5 * 3)) - 2"), Equals("20"));

    CHECK_THAT(eval("7+3*4"), Equals("19"));
    CHECK_THAT(eval("3*7 + 2*4"), Equals("29"));
    CHECK_THAT(eval("-5"), Equals("-5"));
    CHECK_THAT(eval("-1 + 5"), Equals("4"));
    CHECK_THAT(eval("5 + -1"), Equals("4"));
}

TEST_CASE("Eval simple lambda expressions") {
    CHECK_THAT(eval("(fn () 7)"), Equals("7"));
    CHECK_THAT(eval("(fn (x) (* x x x)) 5"), Equals("125"));
    CHECK_THAT(eval("(fn (a b c) (* a (+ b c))) 3 4 6"), Equals("30"));

    CHECK_THAT(eval(R"code(
        = x 7;
        = x (* x 11);
        = x (+ x 3);
        x
    )code"), Equals("(80)"));

    CHECK_THAT(eval(R"code(
        x = 7;
        x = (x * 11);
        x = (x + 3);
        x
    )code"), Equals("(80)"));

    CHECK_THAT(eval(R"code(
        = cube (fn (x) (* x x x));
        (cube 3)
    )code"), Equals("(27)"));

    CHECK_THAT(eval(R"code(
        cube = (fn (x) (* x x x));
        cube 3;
    )code"), Equals("27"));

    // foo = (fn (x) (if x (* x (foo (x - 1))) 1))
    CHECK_THAT(eval(R"code(
        factorial = (fn (n) (if n (* n (factorial (n - 1))) 1));
        factorial 8;
    )code"), Equals("40320"));
    CHECK_THAT(eval("factorial = (fn (x) (if x (* x (factorial (x - 1))) 1)); factorial 10"), Equals("3.6288e+06"));
    CHECK_THAT(eval("factorial = (fn (x) (if x (* x (factorial (x - 1))) 1)); factorial 100"), Equals("9.33262e+157"));

    CHECK_THROWS_WITH(eval("(fn (a b) (+ a b)) 1"), Contains("This function expects 2 arguments"));
    CHECK_THROWS_WITH(eval("(fn (a b) (+ a b)) 1 5 7"), Contains("This function expects 2 arguments"));
}

TEST_CASE("Test Quote") {
    CHECK_THAT(eval("quote (+ 1 2)"), Equals("(+ 1 2)"));
    CHECK_THAT(eval("(quote (+ 1 2))"), Equals("((+ 1 2))"));
    CHECK_THAT(eval("((quote (+ 1 2)))"), Equals("(((+ 1 2)))"));

    auto unexpctedPlus = "Expected a value of type Real, but instead got the value `function<+>`";
    CHECK_THROWS_WITH(eval("2 * quote (+ 1 2)"), Contains(unexpctedPlus));
}

TEST_CASE("Test Reval") {
    CHECK_THAT(eval("2 * (reval (quote (+ 1 2)))"), Equals("6"));
}

TEST_CASE("Test Map") {
    CHECK_THAT(eval("map (a 1) (b 2)"), Equals("{ 'a': 1, 'b': 2 }"));
    CHECK_THAT(eval("map (a (+ 3 4)) (b (* 3 4))"), Equals("{ 'a': 7, 'b': 12 }"));
}
