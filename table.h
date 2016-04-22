//
// Created by Arjun Menon on 4/21/16.
//

#ifndef VECTORCALC_TABLE_H
#define VECTORCALC_TABLE_H

enum CellType {
    NULL_CELL,
    REAL_CELL
};

struct Cell {
    virtual CellType getType() const = 0;

    virtual std::ostream& display(std::ostream &stream) const = 0;
};

std::ostream& operator<<(std::ostream &stream, const Cell &cell);

struct NilCell : Cell {
    virtual CellType getType() const {
        return NULL_CELL;
    }
    virtual std::ostream& display(std::ostream &stream) const;
};

struct RealCell : Cell {
    const real val;

    RealCell(real val) : val(val) {}

    virtual CellType getType() const {
        return REAL_CELL;
    }
    virtual std::ostream& display(std::ostream &stream) const;
};

class Column {
    vector<unique_ptr<Cell>> cells;

    /**
     * Display-related information:
     */
    size_t max_str_len = 4;
    const int extra_padding = 3;

public:
    Column(size_t null_cells = 0) {
        for(size_t i = 0; i < null_cells; i++)
            addNullCell();
    }

    void addCell(unique_ptr<Cell> cell);
    void addNullCell();
    void addRealCell(real val, size_t len);
    const Cell& getCell(const size_t row) const;

    int cell_width();

    static vector<size_t> generate_indices(size_t count);

    std::ostream& display(std::ostream &stream = cout);
};

std::ostream &operator<<(std::ostream& stream, Column& column);

class Table : public DataConsumer {
    size_t row_count = 0;
    size_t col_count = 0;
    vector<unique_ptr<Column>> cols;

    // Represents current column; used for construction
    size_t cur_col = 0;

    inline void addColumn() {
        cols.push_back(unique_ptr<Column>(new Column(row_count - 1)));
    }

public:
    virtual void startRow() override;
    virtual void addReal(real val, size_t len) override;
    virtual void endRow() override;

    inline size_t getRowCount() { return row_count; }
    inline size_t getColCount() { return col_count; }

    Column& getColumn(const size_t col);

    inline Column& operator[](const size_t col) {
        return getColumn(col);
    }

    inline const Cell& getCell(const size_t col, const size_t row) {
        return getColumn(col).getCell(row);
    }

    std::ostream& display(std::ostream &stream, vector<size_t> column_indices);

    inline std::ostream& display(std::ostream &stream = cout) {
        return display(stream, Column::generate_indices(col_count));
    }
};

std::ostream& operator<<(std::ostream &stream, Table &table);

#endif //VECTORCALC_TABLE_H
