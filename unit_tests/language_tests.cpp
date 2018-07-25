#include <catch.hpp>
#include "../src/common.hpp"

using Catch::Matchers::Equals;
using Catch::Matchers::Contains;

string ev(const string& inputExpression) {
    Env env;
    shared_ptr<Value> ast = Parser::parse(inputExpression);
    return toString(*eval(ast, env));
}

TEST_CASE("Eval basic arithmetic expressions") {
    CHECK_THAT(ev(""), Equals("()"));
    CHECK_THAT(ev("+ 1 2 3"), Equals("6"));
    CHECK_THAT(ev("* 4 5"), Equals("20"));
    CHECK_THAT(ev("+ 1 2 (* 4 5) 3 4"), Equals("30"));
}

TEST_CASE("Eval simple lambda expressions") {
    CHECK_THAT(ev("(fn () 7)"), Equals("7"));
    CHECK_THAT(ev("(fn (x) (* x x x)) 5"), Equals("125"));
    CHECK_THAT(ev("(fn (a b c) (* a (+ b c))) 3 4 6"), Equals("30"));

    CHECK_THROWS_WITH(ev("(fn (a b) (+ a b)) 1"), Contains("This functions expects 2 arguments"));
    CHECK_THROWS_WITH(ev("(fn (a b) (+ a b)) 1 5 7"), Contains("This functions expects 2 arguments"));
}
