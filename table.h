//
// Created by Arjun Menon on 4/21/16.
//

#ifndef VECTORCALC_TABLE_H
#define VECTORCALC_TABLE_H

enum CellType {
    NIL,
    INT
};

struct Cell {
    virtual CellType getType() const = 0;
};

struct NilCell : Cell {
    virtual CellType getType() const {
        return NIL;
    }
};

struct IntCell : Cell {
    const int int_val;

    IntCell(int int_val) : int_val(int_val) {}

    virtual CellType getType() const {
        return INT;
    }
};

class Column {
public:
    vector<unique_ptr<Cell>> cells;

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

    void addIntCell(int val, string::size_type len) {
        addCell(unique_ptr<Cell>(dynamic_cast<Cell *>( new IntCell(val) )));
    }

    const Cell& getCell(const size_t row) const {
        return *cells.at(row).get();
    }
};

class Table : public DataConsumer {
private:
    size_t row_count = 0;
    size_t col_count = 0;
    vector<unique_ptr<Column>> cols;

    // Represents current column; used for construction
    size_t cur_col = 0;

    void addColumn() {
        cols.push_back(unique_ptr<Column>(new Column(row_count - 1)));
    }

public:
    virtual void startRow() override {
        row_count++;
    }

    virtual void addInt(int val, string::size_type len) override {
        if(col_count <= cur_col) {
            addColumn();
            col_count++;
        }

        getColumn(cur_col++).addIntCell(val, len);
    }

    virtual void endRow() override {
        while(cur_col < col_count)
            getColumn(cur_col++).addNilCell();

        cur_col = 0;
    }

    inline size_t getRowCount() { return row_count; }
    inline size_t getColCount() { return col_count; }

    Column& getColumn(const size_t col) {
        return *cols.at(col).get();
    }

    const Cell& getCell(const size_t col, const size_t row) {
        return getColumn(col).getCell(row);
    }

    std::ostream& display(std::ostream &stream = cout) {
        for(size_t row = 0; row < row_count; row++) {
            for(size_t col = 0; col < col_count; col++) {
                const Cell& cell = getCell(col, row);
                if(cell.getType() == NIL) {
                    stream << "_ ";
                }
                else if(cell.getType() == INT) {
                    const IntCell& intCell = dynamic_cast<const IntCell&>(cell);
                    stream << intCell.int_val << " ";
                }
            }
            stream << endl;
        }
        return stream;
    }
};

std::ostream& operator<<(std::ostream &stream, Table &table) {
    return table.display(stream);
}

#endif //VECTORCALC_TABLE_H
