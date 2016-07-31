
#ifndef VECTORCALC_COLUMN_H
#define VECTORCALC_COLUMN_H

class Column {
    vector<unique_ptr<Cell>> cells;
    size_t max_str_len = 4;

    size_t realCount = 0;
    unique_ptr<Cell> realMin;
    unique_ptr<Cell> realMax;
    unique_ptr<Cell> realAvg;
    unique_ptr<Cell> realMedian;

    void updateRealMin(real newVal);
    void updateRealMax(real newVal);
    void updateRealAverage(real newVal);
    void updateRealStatistics(real newVal);

    void addCell(unique_ptr<Cell> cell);
    void validateEqualSize(const Column &other) const;
    static unique_ptr<Cell> newRealCell(real val);

public:
    Column(size_t null_cells = 0);
    void addNullCell();
    void addRealCell(real val, size_t len);

    unique_ptr<Column> applyOp(const Column& other, function<unique_ptr<Cell>(const Cell&, const Cell&)> op) const;
    unique_ptr<Column> operator+(const Column& other) const;
    unique_ptr<Column> operator-(const Column& other) const;
    unique_ptr<Column> operator*(const Column& other) const;
    unique_ptr<Column> operator/(const Column& other) const;

    const Cell& getCell(const size_t row) const;

    inline const size_t getSize() const { return cells.size(); }
    inline const Cell& getMin() const { return *realMin; };
    inline const Cell& getMax() const { return *realMax; };
    inline const Cell& getAvg() const { return *realAvg; };

    int cell_width() const;
    std::ostream& display(std::ostream &stream = cout) const;
};

std::ostream &operator<<(std::ostream& stream, Column& column);

#endif //VECTORCALC_COLUMN_H
