//
// Created by Arjun Menon on 4/21/16.
//

#include "stdinclude.h"
#include "data_reader.h"
#include "table.h"

std::ostream &Cell::display(std::ostream &stream) const {
    stream << "NULL";
    return stream;
}

std::ostream &RealCell::display(std::ostream &stream) const {
    stream << val;
    return stream;
}

unique_ptr<Cell> RealCell::operator+(const Cell &other) const {
    if(other.getType() == REAL_CELL)
        return unique_ptr<Cell>(dynamic_cast<Cell *>(new RealCell(val + dynamic_cast<const RealCell&>(other).val)));
    else
        return other + *this;
}

unique_ptr<Cell> RealCell::operator-(const Cell &other) const {
    if(other.getType() == REAL_CELL)
        return unique_ptr<Cell>(dynamic_cast<Cell *>(new RealCell(val - dynamic_cast<const RealCell&>(other).val)));
    else
        return other + *this;
}

unique_ptr<Cell> RealCell::operator*(const Cell &other) const {
    if(other.getType() == REAL_CELL)
        return unique_ptr<Cell>(dynamic_cast<Cell *>(new RealCell(val * dynamic_cast<const RealCell&>(other).val)));
    else
        return other + *this;
}

unique_ptr<Cell> RealCell::operator/(const Cell &other) const {
    if(other.getType() == REAL_CELL)
        return unique_ptr<Cell>(dynamic_cast<Cell *>(new RealCell(val / dynamic_cast<const RealCell&>(other).val)));
    else
        return other + *this;
}

void Column::addCell(unique_ptr<Cell> cell) {
    cells.push_back(move(cell));
}

void Column::addNullCell() {
    addCell(unique_ptr<Cell>(dynamic_cast<Cell *>( new Cell() )));
}

void Column::addRealCell(real val, size_t len) {
    max_str_len = max(max_str_len, len);
    addCell(unique_ptr<Cell>(dynamic_cast<Cell *>( new RealCell(val) )));
}

std::ostream &operator<<(std::ostream &stream, const Cell &cell) {
    return cell.display(stream);
}

struct Counter {
    size_t count = 0;
    size_t operator()() {
        return count++;
    }
};

vector<size_t> generate_indices(size_t count) {
    vector<size_t> column_indexes(count);
    generate_n(column_indexes.begin(), count, Counter());
    return column_indexes;
}

unique_ptr<Column> Column::operator+(const Column &other) const {
    validateEqualSize(other);
    unique_ptr<Column> result = unique_ptr<Column>(new Column);
    for(size_t i = 0; i < getSize(); i++) {
        result->addCell(getCell(i) + other.getCell(i));
    }
    return result;
}

unique_ptr<Column> Column::operator-(const Column &other) const {
    validateEqualSize(other);
    unique_ptr<Column> result = unique_ptr<Column>(new Column);
    for(size_t i = 0; i < getSize(); i++) {
        result->addCell(getCell(i) - other.getCell(i));
    }
    return result;
}

unique_ptr<Column> Column::operator*(const Column &other) const {
    validateEqualSize(other);
    unique_ptr<Column> result = unique_ptr<Column>(new Column);
    for(size_t i = 0; i < getSize(); i++) {
        result->addCell(getCell(i) * other.getCell(i));
    }
    return result;
}

unique_ptr<Column> Column::operator/(const Column &other) const {
    validateEqualSize(other);
    unique_ptr<Column> result = unique_ptr<Column>(new Column);
    for(size_t i = 0; i < getSize(); i++) {
        result->addCell(getCell(i) / other.getCell(i));
    }
    return result;
}

void Column::validateEqualSize(const Column &other) const {
    if(getSize() != other.getSize())
        throw logic_error("");
}

const Cell &Column::getCell(const size_t row) const {
    if(row < cells.size())
        return *cells.at(row).get();
    else
        throw out_of_range(string("Requested row ") + to_string(row) +
                           " does not exist. Number of rows = " + to_string(cells.size()));
}

const size_t Column::getSize() const {
    return cells.size();
}

int Column::cell_width() {
    return static_cast<int>(max_str_len) + extra_padding;
}

std::ostream &Column::display(ostream& stream) {
    for(size_t row = 0; row < cells.size(); row++)
        stream << setw(cell_width()) << getCell(row) << endl;
    return stream;
}

std::ostream &operator<<(std::ostream& stream, Column& column) {
    return column.display(stream);
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

Column &Table::getColumn(const size_t col) {
    if(col < col_count)
        return *cols.at(col).get();
    else
        throw out_of_range(string("Requested column ") + to_string(col) +
                           " does not exist. Number of columns = " + to_string(col_count));

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
