
#ifndef VECTORCALC_CELL_H
#define VECTORCALC_CELL_H

enum CellType {
    NULL_CELL,
    REAL_CELL
};

/*
 * Stores the value of a cell in a column.
 */
struct Cell {
    /*
     * By default, a Cell is NULL.
     */
    virtual CellType getType() const { return NULL_CELL; }

    /*
     * Operations against a NULL cell result in another NULL.
     */
    virtual unique_ptr<Cell> operator+(const Cell &other) const { return unique_ptr<Cell>(new Cell()); }
    virtual unique_ptr<Cell> operator-(const Cell &other) const { return unique_ptr<Cell>(new Cell()); }
    virtual unique_ptr<Cell> operator*(const Cell &other) const { return unique_ptr<Cell>(new Cell()); }
    virtual unique_ptr<Cell> operator/(const Cell &other) const { return unique_ptr<Cell>(new Cell()); }

    virtual std::ostream& display(std::ostream &stream) const;
};

/*
 * This Cell stores a real number.
 */
struct RealCell : Cell {
    const real val;

    RealCell(real val) : val(val) {}

    virtual CellType getType() const { return REAL_CELL; }

    virtual unique_ptr<Cell> operator+(const Cell &other) const;
    virtual unique_ptr<Cell> operator-(const Cell &other) const;
    virtual unique_ptr<Cell> operator*(const Cell &other) const;
    virtual unique_ptr<Cell> operator/(const Cell &other) const;

    virtual std::ostream& display(std::ostream &stream) const;
};

std::ostream& operator<<(std::ostream &stream, const Cell &cell);

#endif //VECTORCALC_CELL_H
