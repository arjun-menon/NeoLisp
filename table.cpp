//
// Created by Arjun Menon on 4/21/16.
//

#include "stdinclude.h"
#include "data_reader.h"
#include "table.h"

std::ostream &operator<<(std::ostream &stream, const Cell &cell) {
    return cell.display(stream);
}

std::ostream &NilCell::display(std::ostream &stream) const {
    stream << "NULL";
    return stream;
}

std::ostream &RealCell::display(std::ostream &stream) const {
    stream << val;
    return stream;
}

void Column::addCell(unique_ptr<Cell> cell) {
    cells.push_back(move(cell));
}

void Column::addNullCell() {
    addCell(unique_ptr<Cell>(dynamic_cast<Cell *>( new NilCell() )));
}

void Column::addRealCell(real val, size_t len) {
    max_str_len = max(max_str_len, len);
    addCell(unique_ptr<Cell>(dynamic_cast<Cell *>( new RealCell(val) )));
}

const Cell &Column::getCell(const size_t row) const {
    if(row < cells.size())
        return *cells.at(row).get();
    else
        throw out_of_range(string("Requested row ") + to_string(row) +
                           " does not exist. Number of rows = " + to_string(cells.size()));
}

Column &Table::getColumn(const size_t col) {
    if(col < col_count)
        return *cols.at(col).get();
    else
        throw out_of_range(string("Requested column ") + to_string(col) +
                           " does not exist. Number of columns = " + to_string(col_count));

}

int Column::cell_width() {
    return static_cast<int>(max_str_len) + extra_padding;
}

struct Counter {
    size_t count = 0;
    size_t operator()() {
        return count++;
    }
};

vector<size_t> Column::generate_indices(size_t count) {
    vector<size_t> column_indexes(count);
    generate_n(column_indexes.begin(), count, Counter());
    return column_indexes;
}

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
        getColumn(cur_col++).addNullCell();

    cur_col = 0;
}

std::ostream &Table::display(ostream& stream, vector<size_t> column_indices) {
    for(size_t row = 0; row < row_count; row++) {
        for(auto col : column_indices)
            stream << setw(getColumn(col).cell_width()) << getCell(col, row);
        stream << endl;
    }
    return stream;
}

std::ostream &operator<<(std::ostream& stream, Table& table) {
    return table.display(stream);
}

std::ostream &Column::display(ostream& stream) {
    for(size_t row = 0; row < cells.size(); row++)
        stream << setw(cell_width()) << getCell(row) << endl;
    return stream;
}

std::ostream &operator<<(std::ostream& stream, Column& column) {
    return column.display(stream);
}
