
#pragma once

class Column {
    vector<Real> cells;

    size_t realCount = 0;
    Real minimum;
    Real maximum;
    Real average;
    Real median;

    void addCell(const Real& cell);

    void updateRealStatistics(Real newVal);
    void updateRealAverage(Real newVal);
    void updateRealMax(Real newVal);
    void updateRealMin(Real newVal);

    void validateEqualSize(const Column &other) const;
    unique_ptr<Column> applyOp(const Column& other,
           function<Real(const Real&, const Real&)> op) const;

 public:
    Column(size_t null_cells = 0);
    void addEmptyCell();
    void addRealCell(const Real &val);

    unique_ptr<Column> operator+(const Column& other) const;
    unique_ptr<Column> operator-(const Column& other) const;
    unique_ptr<Column> operator*(const Column& other) const;
    unique_ptr<Column> operator/(const Column& other) const;

    const Real& getCell(const size_t row) const;

    inline const size_t getSize() const { return cells.size(); }
    inline const Real& getAvg() const { return average; }
    inline const Real& getMax() const { return maximum; }
    inline const Real& getMin() const { return minimum; }

    int cell_width() const;
    ostream& display(ostream &stream = cout) const;
};

inline ostream &operator<<(ostream& stream, Column& column) {
    return column.display(stream);
}
