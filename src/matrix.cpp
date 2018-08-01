#include "common.hpp"

Column::Column(size_t null_cells) {
    for (size_t i = 0; i < null_cells; i++)
        addEmptyCell();
}

void Column::addCell(const Real& cell) {
    if (cell.isValid())
        updateRealStatistics(cell());

    cells.push_back(cell);
}

void Column::updateRealStatistics(Real newVal) {
    // Update statistics on real value cells
    updateRealMin(newVal);
    updateRealMax(newVal);
    updateRealAverage(newVal);
    realCount++;
}

// Update the average as real numbers are added
void Column::updateRealAverage(Real newVal) {
    if (!average.isValid()) {
        average = newVal;
    } else {
        Real oldAvg = average();
        average = (oldAvg * realCount + newVal) / (realCount + 1);
    }
}

// Update the max. as real numbers are added
void Column::updateRealMax(Real newVal) {
    if (!maximum.isValid())
        maximum = newVal;
    else
        maximum = max(maximum(), newVal());
}

// Update the min. as real numbers are added
void Column::updateRealMin(Real newVal) {
    if (!minimum.isValid())
        minimum = newVal;
    else
        minimum = min(minimum(), newVal());
}

void Column::validateEqualSize(const Column &other) const {
    if (getSize() != other.getSize())
        throw logic_error("Cannot perform operations across columns of different lengths.");
}

unique_ptr<Column> Column::applyOp(const Column &other, function<Real(const Real&, const Real&)> op) const {
    validateEqualSize(other);
    unique_ptr<Column> result = unique_ptr<Column>(new Column);
    for (size_t i = 0; i < getSize(); i++) {
        result->addCell(op(getCell(i), other.getCell(i)));
    }
    return result;
}

void Column::addEmptyCell() {
    addCell(Real());
}

void Column::addRealCell(const Real &val) {
    addCell(val);
}

unique_ptr<Column> Column::operator+(const Column &other) const {
    return applyOp(other, [](const Real& a, const Real& b) { return a + b; });
}

unique_ptr<Column> Column::operator-(const Column &other) const {
    return applyOp(other, [](const Real& a, const Real& b) { return a - b; });
}

unique_ptr<Column> Column::operator*(const Column &other) const {
    return applyOp(other, [](const Real& a, const Real& b) { return a * b; });
}

unique_ptr<Column> Column::operator/(const Column &other) const {
    return applyOp(other, [](const Real& a, const Real& b) { return a / b; });
}

const Real& Column::getCell(const size_t row) const {
    if (row < cells.size())
        return cells[row];
    else
        throw out_of_range(string("Requested row ") + to_string(row) +
                           " does not exist. Number of rows = " + to_string(cells.size()));
}

int Column::cell_width() const {
    const int padding = 2;
    const int min_length = 4;  // length of 'NULL'

    return max(static_cast<int>(toString(maximum).length()), min_length) + padding;
}

ostream &Column::display(ostream& stream) const {
    for (size_t row = 0; row < cells.size(); row++)
        stream << setw(cell_width()) << getCell(row) << endl;
    return stream;
}

void CsvReader::process_file(const string &filename) {
    ifstream csv_file(filename);
    if (csv_file.fail())
        throw runtime_error(string("File \"") + filename + "\" does not exist.");

    string line;
    while (getline(csv_file, line)) {
        process_line(line);
    }
}

void CsvReader::process_line(const string &line) {
    bool did_add_item = false;
    const char* num_start_chars = "-1234567890";

    size_t pos = 0;
    while (pos < line.size()) {
        pos = line.find_first_of(num_start_chars, pos);
        if (pos == string::npos)
            break;

        size_t num_len;
        Real val;
        try {
            num_len = val.fromStr(line.c_str() + pos);
        } catch (invalid_argument&) {
            pos++;
            continue;
        }

        if (!did_add_item) {
            dataConsumer.startRow();
            did_add_item = true;
        }

        dataConsumer.addReal(val);

        pos += num_len;
    }

    if (did_add_item) {
        dataConsumer.endRow();
    }
}

void Matrix::startRow() {
    row_count++;
}

void Matrix::addReal(const Real &val) {
    if (col_count <= cur_col) {
        addColumn();
        col_count++;
    }

    cols[cur_col++]->addRealCell(val);
}

void Matrix::endRow() {
    while (cur_col < col_count)
        cols[cur_col++]->addEmptyCell();

    cur_col = 0;
}

const Column &Matrix::getColumn(const size_t col) const {
    if (col < col_count)
        return *cols.at(col);
    else
        throw out_of_range(string("Requested column ") + to_string(col) +
                           " does not exist. Number of columns = " + to_string(col_count));
}

const Real &Matrix::getCell(const size_t col, const size_t row) const {
    return getColumn(col).getCell(row);
}

ostream &Matrix::display(ostream& stream, vector<size_t> column_indices) const {
    for (size_t row = 0; row < row_count; row++) {
        for (auto col : column_indices)
            stream << setw(cols[col]->cell_width()) << getCell(col, row);
        stream << endl;
    }
    return stream;
}
