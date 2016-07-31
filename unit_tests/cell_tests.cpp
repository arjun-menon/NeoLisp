#include "catch.hpp"
#include "../src/stdinclude.h"

TEST_CASE("Null cells display as NULL") {
    Cell nullCell;
    REQUIRE( toString(nullCell) == "NULL" );
}

TEST_CASE("Real cells display their value") {
    RealCell realCell(42.0123);
    REQUIRE( toString(realCell) == "42.0123" );
}

TEST_CASE("Operations against a NULL cell always returns another NULL cell") {
    RealCell realCell(12);
    Cell nullCell;

    REQUIRE( toString(realCell + nullCell) == "NULL" );
    REQUIRE( toString(realCell - nullCell) == "NULL" );
    REQUIRE( toString(realCell / nullCell) == "NULL" );
    REQUIRE( toString(realCell * nullCell) == "NULL" );

    REQUIRE( toString(nullCell + realCell) == "NULL" );
    REQUIRE( toString(nullCell - realCell) == "NULL" );
    REQUIRE( toString(nullCell / realCell) == "NULL" );
    REQUIRE( toString(nullCell * realCell) == "NULL" );
}

TEST_CASE("The RealCell's arithmetic operations work") {
    RealCell a(12);
    RealCell b(3);

    REQUIRE( toString(a + b) == "15" );
    REQUIRE( toString(a - b) == "9" );
    REQUIRE( toString(a / b) == "4" );
    REQUIRE( toString(a * b) == "36" );

    REQUIRE( toString(b + a) == "15" );
    REQUIRE( toString(b - a) == "-9" );
    REQUIRE( toString(b / a) == "0.25" );
    REQUIRE( toString(b * a) == "36" );
}
