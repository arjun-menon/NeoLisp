/*
 * A cell in the table is represented by the `Cell` class here.
 * A concrete implementation of Cell is provided by `RealCell`.
 */

#ifndef VECTORCALC_CELL_H
#define VECTORCALC_CELL_H

enum CellType {
    NULL_CELL,
    REAL_CELL
};

/*
 * Pure virtual interface representing a generic cell.
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
 * Cell that stores a real number (double).
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
