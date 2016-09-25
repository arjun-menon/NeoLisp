#include <catch.hpp>
#include "../src/stdinclude.h"

TEST_CASE("Empty cells display as NULL") {
    Cell emptyCell;
    CHECK( toString(emptyCell) == "NULL" );
}

TEST_CASE("Real cells display their value") {
    Cell realCell(42.0123);
    CHECK( toString(realCell) == "42.0123" );
}

TEST_CASE("Operations against a NULL cell always returns another NULL cell") {
    Cell realCell(12);
    Cell emptyCell;

    CHECK( toString(realCell + emptyCell) == "NULL" );
    CHECK( toString(realCell - emptyCell) == "NULL" );
    CHECK( toString(realCell / emptyCell) == "NULL" );
    CHECK( toString(realCell * emptyCell) == "NULL" );

    CHECK( toString(emptyCell + realCell) == "NULL" );
    CHECK( toString(emptyCell - realCell) == "NULL" );
    CHECK( toString(emptyCell / realCell) == "NULL" );
    CHECK( toString(emptyCell * realCell) == "NULL" );
}

TEST_CASE("The RealCell's arithmetic operations work") {
    Cell a(12);
    Cell b(3);

    CHECK( toString(a + b) == "15" );
    CHECK( toString(a - b) == "9" );
    CHECK( toString(a / b) == "4" );
    CHECK( toString(a * b) == "36" );

    CHECK( toString(b + a) == "15" );
    CHECK( toString(b - a) == "-9" );
    CHECK( toString(b / a) == "0.25" );
    CHECK( toString(b * a) == "36" );
}
