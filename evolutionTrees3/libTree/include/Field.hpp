//
// Created by dym on 01.12.2020.
//

#ifndef DEVTREE_FIELD_HPP
#define DEVTREE_FIELD_HPP

#include <optional>
#include <vector>
#include <forward_list>
#include <ostream>
#include <ch_matrixWrapper.h>
#include "Definitions.hpp"

namespace simulationTree {
class Cell;
namespace def {
using Seeds = std::forward_list<Seed>;
using Trees = std::forward_list<Tree>;
};
using Point = ch::Point<uint16_t, uint8_t, true>;
using Matrix = ch::MatrixWrapper<std::vector<Cell *>, Point>;

class Cell : public def::CellData {
    Matrix::value_type *ptr;
public:
    Cell() = delete;

    template<typename ...Args>
    Cell(Matrix::value_type &inRef, Args &&...args) noexcept
            :ptr(&inRef), def::CellData{std::forward<Args>(args)...} {
        assert(!inRef);
        inRef = this;
    }

    Cell &operator=(Matrix::value_type &inRef) noexcept;

    Cell(Cell &&other) noexcept;

    ~Cell();

    Cell(Cell const &) = delete;

    [[nodiscard]] operator bool() const noexcept;
};

class CellPos : public Cell {
    Point position;
public:
    template<typename ...Args>
    CellPos(Point inP, Args &&...args) noexcept
            : Cell(std::forward<Args>(args)...), position(inP) {}

    [[nodiscard]] operator Point() const noexcept;

    void move(Point pos, Matrix::value_type &ref) noexcept;
};

class Forest {
    Matrix matrix;
public:
    Forest(Point inP);

//    #ifndef NDEBUG
//
//    ~Forest() noexcept(false);
//
//    #endif

    [[nodiscard]] Point getBorder() const noexcept {return matrix.sizePlanar();}

    [[nodiscard]] Cell *const observe(Point inP) const noexcept;

    typedef std::pair<std::optional<CellPos>, Cell const *const> BindResult;

    template<typename ...Args>
    [[nodiscard]] BindResult bind(Point const pos, Args &&... args) noexcept {
        if (auto &cell = matrix[pos])
            return {std::nullopt, cell};
        else
            return {CellPos(pos, cell, std::forward<Args>(args)...), nullptr};
    }

    Cell const *rebind(CellPos &cellPos, Point pos) noexcept;

    [[nodiscard]] Point borders() const noexcept;

    using MatrixForEachPred_test = bool (*)(Matrix::value_type const);

    friend size_t matrixForEach_test(Forest const &f, MatrixForEachPred_test pred);

    enum ConsoleViewMode : uint8_t {
        consoleMinimal = 1 << 0,
        consoleSeeds = 1 << 1,
        consoleNChromo = 1 << 2
    };

    std::ostream &ostream(std::ostream &os, uint8_t mode = consoleMinimal,
                          def::Trees const *treeList = {}, def::Seeds const *seedList = {}) const;

    friend std::ostream &operator<<(std::ostream &os, const Forest &forest);
};
}
#endif //DEVTREE_FIELD_HPP