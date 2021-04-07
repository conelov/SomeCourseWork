//
// Created by dym on 02.12.2020.
//

#ifndef DEVTREE_AREA_HPP
#define DEVTREE_AREA_HPP

#include <ostream>
#include "Seed.hpp"
#include "Tree.hpp"

namespace simulationTree {
class Area {
    Forest forest;
    def::Seeds seeds;
    def::Trees trees;
    std::vector<CellPos *> treeElementsPtr;
    uint64_t iteration{};

    void energyAdd(int8_t e, CellPos &yPos) const noexcept;

public:
    enum class TypeCell : uint8_t {seed = 1, treeActive = 2, treePassive = 3, treeMass};
    struct CellLifeResult {
        TreeBase const *const idPtr;
        Point const position;
        TypeCell const type;
        def::Value const nChromo;
    };

    class Average {
        friend Area;
        size_t totalCount{};
    public:
        Average() noexcept = default;
        Average(def::Energy, size_t, def::Value) noexcept;

        float energy{},
                lenGene{},
                age{};

        void reset() noexcept {*this = Average{};}

    private:
        Average &operator+=(Average other) noexcept;

        void endCalculate() noexcept;
    };

    using LifeResultContainer = std::vector<CellLifeResult>;
    def::Value lossEnergyCellPerTick = 10, solidPower = 5, solidLevel = 2;
    def::Energy energyLimit = 500;
    float buffEnergyLimitFromHeight = 0.05, buffEnergyLimitMaxFromHeight = 0.2;

    Area(Point inP, uint16_t amountSeed, def::ConstantInit inCI = {},
         def::MaxMin<Point::SizeY_type> yPosChance = {0, 0});

    void life(LifeResultContainer *lifeResult = nullptr, Average *average = nullptr);

    [[nodiscard]] bool noLife() const noexcept;

    [[nodiscard]] auto iterations() const noexcept {return iteration;}

    [[nodiscard]] Point borders() const noexcept {return forest.borders();}

    void setConstant(def::Constant);

    std::ostream &ostream(std::ostream &os, uint8_t mode = Forest::ConsoleViewMode::consoleMinimal) const;

    friend std::ostream &operator<<(std::ostream &os, const Area &area);

    friend size_t matrixForEach_test(Area const &f, Forest::MatrixForEachPred_test pred);
};
}
#endif //DEVTREE_AREA_HPP
