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
    table.display(cout, {1, 2});
    cout << endl << table[1] << endl;
    return 0;
}
