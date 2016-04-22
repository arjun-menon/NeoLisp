//
// Created by Arjun Menon on 4/21/16.
//

#include "stdinclude.h"
#include "data_reader.h"
#include "table.h"

void Table::startRow() {
    row_count++;
}

void Table::addReal(real val, size_t len) {
    if(col_count <= cur_col) {
        addColumn();
        col_count++;
    }

    getColumn(cur_col++).addRealCell(val, len);
}

void Table::endRow() {
    while(cur_col < col_count)
        getColumn(cur_col++).addNilCell();

    cur_col = 0;
}

std::ostream &Table::display(ostream &stream) {
    for(size_t row = 0; row < row_count; row++) {
        for(size_t col = 0; col < col_count; col++) {
            const Cell& cell = getCell(col, row);
            const string::size_type width = getColumn(col).max_str_len;
            if(cell.getType() == NULL_CELL) {
                stream << "_ ";
            }
            else if(cell.getType() == REAL_CELL) {
                const RealCell& realCell = dynamic_cast<const RealCell&>(cell);
                stream << setw(width + 2) << realCell.val << " ";
            }
        }
        stream << endl;
    }
    return stream;
}

std::ostream &operator<<(std::ostream &stream, Table &table) {
    return table.display(stream);
}
