#include <catch.hpp>
#include "../src/common.hpp"

using Catch::Matchers::Equals;
using Catch::Matchers::Contains;

static string eval(const string &inputExpression) {
    Env env;
    return toString(*eval(parse(inputExpression), env));
}

static string eval_and_get_r(std::initializer_list<string> inputExpressions)
{
    Env env;
    for (const auto &expr: inputExpressions) {
        eval(parse(expr), env);
    }
    return toString(*env.get(Symbol::create("r")));
}

TEST_CASE("Eval basic arithmetic expressions") {
    CHECK_THAT(eval(""), Equals("()"));
    CHECK_THAT(eval("+ 1 2 3"), Equals("6"));
    CHECK_THAT(eval("* 4 5"), Equals("20"));
    CHECK_THAT(eval("+ 1 2 (* 4 5) 3 4"), Equals("30"));
}

TEST_CASE("Eval simple lambda expressions") {
    CHECK_THAT(eval("(fn () 7)"), Equals("7"));
    CHECK_THAT(eval("(fn (x) (* x x x)) 5"), Equals("125"));
    CHECK_THAT(eval("(fn (a b c) (* a (+ b c))) 3 4 6"), Equals("30"));

    CHECK_THAT(eval_and_get_r({
        "= x 7",
        "= x (* x 11)",
        "= x (+ x 3)",
        "= r x"}),
    Equals("80"));

    CHECK_THAT(eval_and_get_r({
        "= cube (fn (x) (* x x x))",
        "= r (cube 3)"}),
    Equals("27"));

    CHECK_THROWS_WITH(eval("(fn (a b) (+ a b)) 1"), Contains("This functions expects 2 arguments"));
    CHECK_THROWS_WITH(eval("(fn (a b) (+ a b)) 1 5 7"), Contains("This functions expects 2 arguments"));
}
