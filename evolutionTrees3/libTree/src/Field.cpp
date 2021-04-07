//
// Created by dym on 01.12.2020.
//

#include "Field.hpp"

using namespace simulationTree;

Cell &Cell::operator=(Cell *&inRef) noexcept {
    assert(!inRef);
    *ptr = nullptr;
    ptr = &inRef;
    *ptr = this;
    return *this;
}

Cell::Cell(Cell &&other) noexcept
        : def::CellData(std::move(other)),
          ptr(other.ptr) {
    assert(ptr);
    assert(*ptr == &other);
    other.ptr = nullptr;
    *ptr = this;
}

Cell::operator bool() const noexcept {
    if (ptr) {
        assert(*ptr == this);
        return true;
    } else
        return false;
}

Cell::~Cell() {
    if (operator bool())
        *ptr = nullptr;
}

void CellPos::move(Point const pos, Cell *&ref) noexcept {
    Cell::operator=(ref);
    position = pos;
}

CellPos::operator Point() const noexcept {
    return position;
}

Forest::Forest(Point inP)
        : matrix(inP, nullptr) {}

Cell *const Forest::observe(Point inP) const noexcept {
    return matrix.operator[](inP);
}

Cell const *Forest::rebind(CellPos &cellPos, Point pos) noexcept {
    if (auto &cell = matrix[pos];
            !cell) {
        cellPos.move(pos, cell);
        return nullptr;
    } else
        return cell;
}

Point Forest::borders() const noexcept {
    return matrix.sizePlanar();
}

#include <deque>
#include <algorithm>
#include "Seed.hpp"
#include "Tree.hpp"

std::ostream &
Forest::ostream(std::ostream &os, uint8_t mode,
                const def::Trees *const treeList, const def::Seeds *const seedList) const {
    auto const bord = borders();
    assert(bord.x < 101 && bord.y < 101);
    os << BLACK;
    if ((bord.y - 1) / 10)
        os << ' ';
    os << "  ";
    for (Point::SizeX_type i{}; i < bord.x; ++i) {
        if (!(i / 10))
            os << ' ';
        os << static_cast<size_t>( i);
    }
    os << '\n';

    for (Point::SizeY_type j = bord.y - 1;
         j != std::numeric_limits<Point::SizeY_type>::max(); --j) {
        if (!(j / 10) && (bord.y - 1) / 10)
            os << ' ';
        os << static_cast<size_t>(j);
        os << ' ';
        for (Point::SizeX_type i{}; i < bord.x; ++i) {
            Point const pos{i, j};
            if (matrix[pos])
                if (matrix[pos]->treeBase) {
                    auto const nChromo = matrix[pos]->nChromo;
                    if (!(nChromo / 10)) os << ' ';
                    if (seedList && seedList->cend() != std::ranges::find(*seedList, pos))
                        os << static_cast<size_t>( matrix[pos]->nChromo);
                    else
                        os << RESET << static_cast<size_t>( matrix[pos]->nChromo) << BLACK;
                } else
                    os << " X";
            else
                os << " .";
        }
        os << '\n';
    }
    os << RESET;

    if (treeList)
        for (Tree const &i : *treeList) {
            for (CellPos const &j : i.activeE) {
                os << j.operator Point();
                if (mode&consoleNChromo) os << static_cast<uint16_t>(j.nChromo);
            }
            for (CellPos const &j : i.passiveE) {
                os << 'p' << j.operator Point();
                if (mode&consoleNChromo) os << static_cast<uint16_t>(j.nChromo);
            }
            for (CellPos const &j:i.massE) {
                os << 'm' << j.operator Point();
                if (mode&consoleNChromo) os << static_cast<uint16_t>(j.nChromo);
            }
            os << 'e' << i.energy << 'a' << static_cast<int32_t>(i.age) << '\n';
        }
    if (mode&consoleSeeds && seedList)
        for (auto &i:*seedList)
            os << 's' << i.operator Point() << 'e' << i.energy << '\n';
    return os;
}

namespace simulationTree {
std::ostream &operator<<(std::ostream &os, const Forest &forest) {
    return forest.ostream(os, Forest::ConsoleViewMode::consoleSeeds|Forest::ConsoleViewMode::consoleNChromo);
}

//#ifndef NDEBUG
//
//Forest::~Forest() noexcept(false) {
//    constexpr auto each = [](Cell const *const i) {return i != nullptr;};
//    if (auto const countNotNull =
//                std::ranges::count_if(matrix.getContainer(), each);
//            countNotNull != 0) {
//        using namespace std::literals;
//        throw std::runtime_error("Forest::~Forest(): not all cell not null. Count cell: "s +
//                                 std::to_string(countNotNull));
//    }
//}
//
//#endif

size_t matrixForEach_test(const Forest &f, Forest::MatrixForEachPred_test pred) {
    return std::count_if(f.matrix.getContainer().cbegin(), f.matrix.getContainer().cend(), pred);
}

}
