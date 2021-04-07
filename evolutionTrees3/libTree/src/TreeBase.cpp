//
// Created by dym on 16.11.2020.
//

#include <ch_random.h>
#include <TreeBase.hpp>

namespace simulationTree {
constexpr auto RG = [](def::Value min, def::Value max) noexcept {
    return std::uniform_int_distribution<def::Value>(min, max)(ch::RG::get());
};

def::ConstantInit::operator Constant() const {
    if (!(chromoMutatChance.min != 0 && chromoMutatChance.min <= chromoMutatChance.max) ||
        !(chromoMutatTrueChance.min != 0 && chromoMutatTrueChance.min <= chromoMutatTrueChance.max))
        throw std::invalid_argument("ConstantInit::operator Constant(): invalid variant chance");
    return {.chromoMutatChance = RG(chromoMutatChance.min, chromoMutatChance.max),
            .chromoMutatTrueChance = RG(chromoMutatTrueChance.min, chromoMutatTrueChance.max),
            .chromoNewLenChance = RG(chromoNewLenChance.min, chromoNewLenChance.max),
            .ageInit = age};
}

TreeBase::TreeBase(decltype(treeData) inTD, decltype(energy) const inEenergy) noexcept
        : treeData(inTD),
          genomeMono(genomeBuffer, sizeof(genomeBuffer)),
          genome(&genomeMono),
          energy(inEenergy),
          age(treeData.ageInit) {}

TreeBase::TreeBase(def::TreeDataInit const init)
        : TreeBase(init.operator def::TreeData(),
                   init.energy) {
    correctConstant(treeData);
    genome.resize(RG(init.chromoLen.min, init.chromoLen.max));
    for (auto &i : genome)
        for (auto &j : i)
            j = RG(0, (genome.size() - 1) * treeData.chromoMutatTrueChance);
}

TreeBase::TreeBase(TreeBase const &other, def::Energy const inE)
#ifndef NDEBUG
noexcept
#endif
        : TreeBase(other.treeData, inE) {
    assert(!other.genome.empty());
    if (treeData.chromoMutatChance == def::compileConstant.valueOff &&
        treeData.chromoNewLenChance == def::compileConstant.valueOff) {
        genome = other.genome;
        return;
    }
    def::Value const oGenomeLen = other.genome.size(),
            geneMutatN = treeData.chromoMutatChance == def::compileConstant.valueOff
                         ? def::compileConstant.valueOff
                         : RG(0, (oGenomeLen - 1) * treeData.chromoMutatChance);
    if (treeData.chromoNewLenChance != def::compileConstant.valueOff && geneMutatN >= oGenomeLen) {
        auto deltaNGene = RG(0, oGenomeLen - 1);
        if (def::Value const lenNewFlag =
                    treeData.chromoNewLenChance >= 2 ? RG(0, treeData.chromoNewLenChance) : treeData.chromoNewLenChance;
                lenNewFlag % 2) {
            genome.reserve(oGenomeLen + 1);
            for (def::Value i{}; i < oGenomeLen; ++i) {
                genome.push_back(other.genome[i]);
                if (i == deltaNGene)
                    genome.push_back(other.genome[i]);
            }
        } else {
            genome.reserve(oGenomeLen - 1);
            for (def::Value i{}; i < oGenomeLen; ++i)
                if (i != deltaNGene)
                    genome.push_back(other.genome[i]);
        }
    } else {
        genome = other.genome;
        if (geneMutatN >= oGenomeLen)
            return;
        genome.at(geneMutatN)[RG(0, def::compileConstant.chromoSize - 1)] =
                treeData.chromoMutatTrueChance == def::compileConstant.valueOff
                ? def::compileConstant.valueOff
                : RG(0, (oGenomeLen - 1) * treeData.chromoMutatTrueChance);
    }
    assert(!genome.empty());
}

TreeBase::TreeBase(TreeBase &&inTB, def::Energy inE) NOEXCEPT_RELEASE
        : TreeBase(inTB.treeData, inE) {
    assert(!inTB.genome.empty());
    genome = std::move(inTB.genome);
    assert(!genome.empty());
}

TreeBase::TreeBase(TreeBase &&inTB) noexcept
        : TreeBase(std::move(inTB), inTB.energy) {}

void TreeBase::setConstant(def::Constant const inC) {
    correctConstant(inC);
    *static_cast<def::Constant *>(&treeData) = inC;
}

void TreeBase::correctConstant(def::Constant inC) {
    if (inC.chromoMutatTrueChance == 0
        || inC.chromoMutatChance == 0)
        throw std::invalid_argument("TreeBase::correctConstant: ");
}
}