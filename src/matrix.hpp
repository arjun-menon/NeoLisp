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

/**
 * The consumer implements these callbacks.
 *
 *  startRow -- called before every row is processed
 *  addReal -- called for every number in the row
 *  endRow -- called after each row is processed
 */
class CsvConsumer {
public:
    virtual void startRow() {}
    virtual void addReal(const Real &val) = 0;
    virtual void endRow() {}
};

/**
 * Reads in a file containing lines of multiple numbers, e.g. a CSV of numbers.
 */
class CsvReader {
    CsvConsumer &dataConsumer;
    CsvReader(CsvConsumer &dataConsumer) : dataConsumer(dataConsumer) {}

    void process_file(const string &filename);
    void process_line(const string &line);

public:
    inline static void read_file(const string &filename, CsvConsumer &dataConsumer) {
        CsvReader(dataConsumer).process_file(filename);
    }
};

class Matrix : public CsvConsumer {
    size_t row_count = 0;
    size_t col_count = 0;
    vector< unique_ptr<Column> > cols;

    size_t cur_col = 0;  // Represents current column; used for construction

    inline void addColumn() {
        cols.push_back(unique_ptr<Column>(new Column(row_count - 1)));
    }

 public:
    void startRow() override;
    void addReal(const Real &val) override;
    void endRow() override;

    inline size_t getRowCount() const { return row_count; }
    inline size_t getColCount() const { return col_count; }

    const Column& getColumn(const size_t col) const;
    inline const Column& operator[](const size_t col) const { return getColumn(col); }
    const Real& getCell(const size_t col, const size_t row) const;

    ostream& display(ostream &stream, vector<size_t> column_indices) const;

    inline ostream& display(ostream &stream = cout) const  {
        return display(stream, generateSequence<size_t>(col_count));
    }
};

inline ostream& operator<<(ostream &stream, Matrix &matrix) {
    return matrix.display(stream);
}
