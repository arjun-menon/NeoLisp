
#ifndef VECTORCALC_COLUMN_H
#define VECTORCALC_COLUMN_H

class Column {
    vector<Cell> cells;
    size_t max_str_len = 4;

    size_t realCount = 0;
    Cell minimum;
    Cell maximum;
    Cell average;
    Cell median;

    void updateRealMin(real newVal);
    void updateRealMax(real newVal);
    void updateRealAverage(real newVal);
    void updateRealStatistics(real newVal);

    void validateEqualSize(const Column &other) const;

public:
    Column(size_t null_cells = 0);
    void addEmptyCell();
    void addRealCell(real val, size_t len);
    void addCell(const Cell& cell);

    unique_ptr<Column> applyOp(const Column& other, function<Cell(const Cell&, const Cell&)> op) const;
    unique_ptr<Column> operator+(const Column& other) const;
    unique_ptr<Column> operator-(const Column& other) const;
    unique_ptr<Column> operator*(const Column& other) const;
    unique_ptr<Column> operator/(const Column& other) const;

    const Cell& getCell(const size_t row) const;

    inline const size_t getSize() const { return cells.size(); }
    inline const Cell& getMin() const { return minimum; };
    inline const Cell& getMax() const { return maximum; };
    inline const Cell& getAvg() const { return average; };

    int cell_width() const;
    ostream& display(ostream &stream = cout) const;
};

inline ostream &operator<<(ostream& stream, Column& column) {
    return column.display(stream);
}

#endif //VECTORCALC_COLUMN_H
