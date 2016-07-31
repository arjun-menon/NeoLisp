#include "stdinclude.h"

std::ostream& NullCell::display(std::ostream &stream) const {
    stream << "NULL";
    return stream;
}

std::ostream& RealCell::display(std::ostream &stream) const {
    stream << val;
    return stream;
}

unique_ptr<Cell> RealCell::operator+(const Cell &other) const {
    if(other.getType() == REAL_CELL)
        return unique_ptr<Cell>(dynamic_cast<Cell *>(new RealCell(val + dynamic_cast<const RealCell&>(other).val)));
    else
        return other + *this;
}

unique_ptr<Cell> RealCell::operator-(const Cell &other) const {
    if(other.getType() == REAL_CELL)
        return unique_ptr<Cell>(dynamic_cast<Cell *>(new RealCell(val - dynamic_cast<const RealCell&>(other).val)));
    else
        return other + *this;
}

unique_ptr<Cell> RealCell::operator*(const Cell &other) const {
    if(other.getType() == REAL_CELL)
        return unique_ptr<Cell>(dynamic_cast<Cell *>(new RealCell(val * dynamic_cast<const RealCell&>(other).val)));
    else
        return other + *this;
}

unique_ptr<Cell> RealCell::operator/(const Cell &other) const {
    if(other.getType() == REAL_CELL)
        return unique_ptr<Cell>(dynamic_cast<Cell *>(new RealCell(val / dynamic_cast<const RealCell&>(other).val)));
    else
        return other + *this;
}

std::ostream& operator<<(std::ostream &stream, const Cell &cell) {
    return cell.display(stream);
}

std::ostream& operator<<(std::ostream &stream, const unique_ptr<Cell> &cell) {
    return cell->display(stream);
}
