//
// Created by dym on 16.11.2020.
//

#ifndef DEVTREE_DEFINITIONS_HPP
#define DEVTREE_DEFINITIONS_HPP
namespace simulationTree {
class TreeBase;

class Seed;

class Tree;
namespace def {
using Value = uint8_t;
using Energy = int16_t;
template<typename Max, typename Min = Max>
struct MaxMin {
    Max max;
    Min min;
};
struct CellData {
    TreeBase *treeBase;
    def::Value nChromo;
};
struct CompileConstant {
  MaxMin<Value> chromoLen{22, 6},
            value{9, 1};
    Value valueOff = std::numeric_limits<Value>::max(),
            chromoSize = 4,
            initialNChromo = 0;
    uint16_t ActiveSize = 1'000,
            PassiveSize = 100,
            massSize = 5'000;
    Energy energyMax = 20'000;
} constexpr compileConstant;
struct Resources {};
struct Constant {
    def::Value chromoMutatChance,
            chromoMutatTrueChance,
            chromoNewLenChance,
            ageInit;
};
struct TreeData : Resources, Constant {};

struct ConstantInit {
    def::MaxMin<def::Value> chromoLen = {12, 8},
            chromoMutatChance = {4, 4},
            chromoMutatTrueChance = chromoMutatChance,
            chromoNewLenChance = {20, 20};
    def::Value energy = 90,
            age = 20;

    [[nodiscard]] operator Constant() const;
};

struct TreeDataInit : Resources, ConstantInit {
    operator TreeData() const {
        return {*this, *this};}
};
}
}
#endif //DEVTREE_DEFINITIONS_HPP