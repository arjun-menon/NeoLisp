#include "stdinclude.h"

static const int extra_padding = 3;

Column::Column(size_t null_cells) {
    for(size_t i = 0; i < null_cells; i++)
        addNullCell();
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

void Column::validateEqualSize(const Column &other) const {
    if(getSize() != other.getSize())
        throw logic_error("Cannot add columns of different length");
}

unique_ptr<Column> Column::applyOp(const Column &other, function<unique_ptr<Cell>(const Cell&, const Cell&)> op) const {
    validateEqualSize(other);
    unique_ptr<Column> result = unique_ptr<Column>(new Column);
    for(size_t i = 0; i < getSize(); i++) {
        result->addCell( op(getCell(i), other.getCell(i)) );
    }
    return result;
}

unique_ptr<Column> Column::operator+(const Column &other) const {
    return applyOp(other, [](const Cell& a, const Cell& b) { return a + b; });
}

unique_ptr<Column> Column::operator-(const Column &other) const {
    return applyOp(other, [](const Cell& a, const Cell& b) { return a - b; });
}

unique_ptr<Column> Column::operator*(const Column &other) const {
    return applyOp(other, [](const Cell& a, const Cell& b) { return a * b; });
}

unique_ptr<Column> Column::operator/(const Column &other) const {
    return applyOp(other, [](const Cell& a, const Cell& b) { return a / b; });
}

const Cell &Column::getCell(const size_t row) const {
    if(row < cells.size())
        return *cells[row].get();
    else
        throw out_of_range(string("Requested row ") + to_string(row) +
                           " does not exist. Number of rows = " + to_string(cells.size()));
}

const size_t Column::getSize() const {
    return cells.size();
}

int Column::cell_width() const {
    return static_cast<int>(max_str_len) + extra_padding;
}

std::ostream &Column::display(ostream& stream) const {
    for(size_t row = 0; row < cells.size(); row++)
        stream << setw(cell_width()) << getCell(row) << endl;
    return stream;
}

std::ostream &operator<<(std::ostream& stream, Column& column) {
    return column.display(stream);
}
