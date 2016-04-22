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
};

struct NilCell : Cell {
    virtual CellType getType() const {
        return NULL_CELL;
    }
};

struct RealCell : Cell {
    const real val;

    RealCell(real val) : val(val) {}

    virtual CellType getType() const {
        return REAL_CELL;
    }
};

class Column {
    vector<unique_ptr<Cell>> cells;

public:
    size_t max_str_len = 0;

    Column(size_t nil_cells = 0) {
        for(size_t i = 0; i < nil_cells; i++)
            addNilCell();
    }

    void addCell(unique_ptr<Cell> cell) {
        cells.push_back(move(cell));
    }

    void addNilCell() {
        addCell(unique_ptr<Cell>(dynamic_cast<Cell *>( new NilCell() )));
    }

    void addRealCell(real val, size_t len) {
        max_str_len = max(max_str_len, len);
        addCell(unique_ptr<Cell>(dynamic_cast<Cell *>( new RealCell(val) )));
    }

    const Cell& getCell(const size_t row) const {
        return *cells.at(row).get();
    }
};

class Table : public DataConsumer {
    size_t row_count = 0;
    size_t col_count = 0;
    vector<unique_ptr<Column>> cols;

    // Represents current column; used for construction
    size_t cur_col = 0;

    void addColumn() {
        cols.push_back(unique_ptr<Column>(new Column(row_count - 1)));
    }

public:
    virtual void startRow() override;
    virtual void addReal(real val, size_t len) override;
    virtual void endRow() override;

    inline size_t getRowCount() { return row_count; }
    inline size_t getColCount() { return col_count; }

    Column& getColumn(const size_t col) {
        return *cols.at(col).get();
    }

    const Cell& getCell(const size_t col, const size_t row) {
        return getColumn(col).getCell(row);
    }

    std::ostream& display(std::ostream &stream = cout);
};

std::ostream& operator<<(std::ostream &stream, Table &table);

#endif //VECTORCALC_TABLE_H
