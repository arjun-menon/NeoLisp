/**
 * Main entry point.
 */

#include "stdinclude.h"
#include "data_reader.h"
#include "table.h"

int main() {
    Table table;
    DataReader::read_file("test_data/t1.txt", table);
    table.display();
    return 0;
}
