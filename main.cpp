/**
 * Main entry point.
 */

#include "stdinclude.h"
#include "data_reader.h"
#include "table.h"

int main() {
    Table table;
    DataReader::read_file("test_data/t1.txt", table);
    cout << "table" << endl << table << endl;
    table.display(cout, {1, 3});
    const Column &c1 = table[1];
    const Column &c3 = table[3];
    cout << endl << *(c1 / c3) << endl;
    return 0;
}
