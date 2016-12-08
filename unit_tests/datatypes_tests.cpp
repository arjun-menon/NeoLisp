#include <catch.hpp>
#include "../src/common.h"

TEST_CASE("Real displays its value properly") {
    Real validReal(42.0123);
    CHECK( toString(validReal) == "42.0123" );
}

TEST_CASE("An uninitialized/empty Real is NaN, and displays itself as NaN") {
    Real nanReal;
    CHECK( toString(nanReal) == "NaN" );
}

TEST_CASE("Operations against a NaN always returns another Nan") {
    Real validReal(12);
    Real nanReal;

    CHECK( toString(validReal + nanReal) == "NaN" );
    CHECK( toString(validReal - nanReal) == "NaN" );
    CHECK( toString(validReal / nanReal) == "NaN" );
    CHECK( toString(validReal * nanReal) == "NaN" );

    CHECK( toString(nanReal + validReal) == "NaN" );
    CHECK( toString(nanReal - validReal) == "NaN" );
    CHECK( toString(nanReal / validReal) == "NaN" );
    CHECK( toString(nanReal * validReal) == "NaN" );
}

TEST_CASE("The Real's arithmetic operations work") {
    Real a(12);
    Real b(3);

    CHECK( toString(a + b) == "15" );
    CHECK( toString(a - b) == "9" );
    CHECK( toString(a / b) == "4" );
    CHECK( toString(a * b) == "36" );

    CHECK( toString(b + a) == "15" );
    CHECK( toString(b - a) == "-9" );
    CHECK( toString(b / a) == "0.25" );
    CHECK( toString(b * a) == "36" );
}
