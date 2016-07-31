#include "stdinclude.h"

static const int extra_padding = 3;

Column::Column(size_t null_cells) :
        realMin(new NullCell()), realMax(new NullCell()),
        realAvg(new NullCell()), realMedian(new NullCell()) {
    for(size_t i = 0; i < null_cells; i++)
        addNullCell();
}

// Update the min. as real numbers are added
void Column::updateRealMin(real newVal) {
    if(realMin->isPresent() == ABSENT) {
        realMin = newRealCell(newVal);
    }
    else if(realMin->isPresent() == PRESENT) {
        realMin = newRealCell(min(dynamic_cast<RealCell *>(realMin.get())->val, newVal));
    }
}

// Update the max. as real numbers are added
void Column::updateRealMax(real newVal) {
    if(realMax->isPresent() == ABSENT) {
        realMax = newRealCell(newVal);
    }
    else if(realMax->isPresent() == PRESENT) {
        realMax = newRealCell(max(dynamic_cast<RealCell *>(realMax.get())->val, newVal));
    }
}

// Update the average as real numbers are added
void Column::updateRealAverage(real newVal) {
    if(realAvg->isPresent() == ABSENT) {
        realAvg = newRealCell(newVal);
    }
    else if(realAvg->isPresent() == PRESENT) {
        real oldAvg = dynamic_cast<RealCell *>(realAvg.get())->val;
        real newAvg = (oldAvg * realCount + newVal) / (realCount + 1);
        realAvg = newRealCell(newAvg);
    }
}

void Column::updateRealStatistics(real newVal) {
    // Update statistics on real value cells
    updateRealMin(newVal);
    updateRealMax(newVal);
    updateRealAverage(newVal);
    realCount++;
}

void Column::addCell(unique_ptr<Cell> cell) {
    if(cell->isPresent() == PRESENT) {
        updateRealStatistics(dynamic_cast<RealCell*>(cell.get())->val);
    }

    cells.push_back(move(cell));
}

void Column::validateEqualSize(const Column &other) const {
    if(getSize() != other.getSize())
        throw logic_error("Cannot add columns of different length");
}

unique_ptr<Cell> Column::newRealCell(real val) {
    return unique_ptr<Cell>(dynamic_cast<Cell *>(new RealCell(val)));
}

void Column::addNullCell() {
    addCell(unique_ptr<Cell>(new NullCell()));
}

void Column::addRealCell(real val, size_t len) {
    max_str_len = max(max_str_len, len);
    addCell(newRealCell(val));
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
