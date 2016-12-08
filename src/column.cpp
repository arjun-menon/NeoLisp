#include "common.h"

Column::Column(size_t null_cells) {
    for(size_t i = 0; i < null_cells; i++)
        addEmptyCell();
}

void Column::addCell(const Cell& cell) {
    if(cell.isPresent())
        updateRealStatistics(cell.get());

    cells.push_back(cell);
}

void Column::updateRealStatistics(real newVal) {
    // Update statistics on real value cells
    updateRealMin(newVal);
    updateRealMax(newVal);
    updateRealAverage(newVal);
    realCount++;
}

// Update the average as real numbers are added
void Column::updateRealAverage(real newVal) {
    if(!average.isPresent())
        average = newVal;
    else {
        real oldAvg = average.get();
        average = (oldAvg * realCount + newVal) / (realCount + 1);
    }
}

// Update the max. as real numbers are added
void Column::updateRealMax(real newVal) {
    if(!maximum.isPresent())
        maximum = newVal;
    else
        maximum = max(maximum.get(), newVal);
}

// Update the min. as real numbers are added
void Column::updateRealMin(real newVal) {
    if(!minimum.isPresent())
        minimum = newVal;
    else
        minimum = min(minimum.get(), newVal);
}

void Column::validateEqualSize(const Column &other) const {
    if(getSize() != other.getSize())
        throw logic_error("Cannot perform operations across columns of different lengths.");
}

unique_ptr<Column> Column::applyOp(const Column &other, function<Cell(const Cell&, const Cell&)> op) const {
    validateEqualSize(other);
    unique_ptr<Column> result = unique_ptr<Column>(new Column);
    for(size_t i = 0; i < getSize(); i++) {
        result->addCell( op(getCell(i), other.getCell(i)) );
    }
    return result;
}

void Column::addEmptyCell() {
    addCell(Cell());
}

void Column::addRealCell(real val) {
    addCell(Cell(val));
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

const Cell& Column::getCell(const size_t row) const {
    if(row < cells.size())
        return cells[row];
    else
        throw out_of_range(string("Requested row ") + to_string(row) +
                           " does not exist. Number of rows = " + to_string(cells.size()));
}

int Column::cell_width() const {
    const int padding = 2;
    const int min_length = 4; // length of 'NULL'

    return max((int) toString(maximum).length(), min_length) + padding;
}

ostream &Column::display(ostream& stream) const {
    for(size_t row = 0; row < cells.size(); row++)
        stream << setw(cell_width()) << getCell(row) << endl;
    return stream;
}
