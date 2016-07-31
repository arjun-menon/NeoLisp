
#ifndef VECTORCALC_TABLE_H
#define VECTORCALC_TABLE_H

class Table : public DataConsumer {
    size_t row_count = 0;
    size_t col_count = 0;
    vector<unique_ptr<Column>> cols;

    size_t cur_col = 0; // Represents current column; used for construction

    inline void addColumn() {
        cols.push_back(unique_ptr<Column>(new Column(row_count - 1)));
    }

public:
    virtual void startRow() override;
    virtual void addReal(real val, size_t len) override;
    virtual void endRow() override;

    inline size_t getRowCount() const { return row_count; }
    inline size_t getColCount() const { return col_count; }

    const Column& getColumn(const size_t col) const;
    inline const Column& operator[](const size_t col) const { return getColumn(col); }
    const Cell& getCell(const size_t col, const size_t row) const;

    std::ostream& display(std::ostream &stream, vector<size_t> column_indices) const;

    inline std::ostream& display(std::ostream &stream = cout) const  {
        return display(stream, generateSequence<size_t>(col_count));
    }
};

std::ostream& operator<<(std::ostream &stream, Table &table);

#endif //VECTORCALC_TABLE_H
