
#ifndef VECTORCALC_COLUMN_H
#define VECTORCALC_COLUMN_H

class Column {
    vector<unique_ptr<Cell>> cells;
    size_t max_str_len = 4;

public:
    Column(size_t null_cells = 0);
    void addCell(unique_ptr<Cell> cell);
    void addNullCell();
    void addRealCell(real val, size_t len);

    void validateEqualSize(const Column &other) const;
    unique_ptr<Column> applyOp(const Column& other, function<unique_ptr<Cell>(const Cell&, const Cell&)> op) const;
    unique_ptr<Column> operator+(const Column& other) const;
    unique_ptr<Column> operator-(const Column& other) const;
    unique_ptr<Column> operator*(const Column& other) const;
    unique_ptr<Column> operator/(const Column& other) const;

    const Cell& getCell(const size_t row) const;
    const size_t getSize() const;

    int cell_width() const;
    std::ostream& display(std::ostream &stream = cout) const;
};

std::ostream &operator<<(std::ostream& stream, Column& column);

#endif //VECTORCALC_COLUMN_H
