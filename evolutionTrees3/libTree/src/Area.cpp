//
// Created by dym on 02.12.2020.
//

#include <ch_random.h>
#include "Area.hpp"

using namespace simulationTree;

Area::Area(Point const inP, uint16_t const amountSeed, def::ConstantInit const inCI,
           def::MaxMin<Point::SizeY_type> const yPosChance)
        : forest(inP) {
    assert(yPosChance.max >= yPosChance.min);
    ch::RandomUnique<Point::SizeX_type> randomUnique(0, inP.x - 1);
    for (uint16_t i{}; i < amountSeed; ++i)
        seeds.emplace_front(forest.bind({randomUnique(), yPosChance.max == 0 && yPosChance.min == yPosChance.max
                                                         ? static_cast<Point::SizeY_type>(0)
                                                         : std::uniform_int_distribution<Point::SizeY_type>(
                        yPosChance.min, yPosChance.max)(ch::RG::get())}),
                            def::TreeDataInit{{}, inCI});
}

#include <algorithm>

void Area::life(LifeResultContainer *const lifeResult, Average *const average) {
    if (noLife())
        throw std::runtime_error("no Life");
    if (lifeResult && !lifeResult->empty())
        throw std::runtime_error("not empty container passed");

    ++iteration;
    ////Обработка семян
    seeds.sort([](Seed const &lhs, Seed const &rhs) noexcept {
        assert(lhs.operator Point() != rhs.operator Point());
        return lhs.operator Point().y <= rhs.operator Point().y;
    });
    for (auto iprev = seeds.before_begin(), icurr = std::next(iprev); icurr != seeds.end();) {
        if (average)
            *average += Average(icurr->energy, icurr->genome.size(), icurr->age);
        ////Если добрался до земли
        if (auto const currPos = icurr->operator Point();
                currPos.y == 0)
            ////Мувнуть в дерево
            trees.emplace_front(std::move(*icurr));
            ////Иначе если под семенем пусто
        else if (auto expected = forest.rebind(*icurr, currPos.shift(Point::down, 1));
                !expected) {
            ////Перемещение выполнено
            if (lifeResult)
                lifeResult->push_back({&*icurr, icurr->operator Point(), TypeCell::seed, 0});
            iprev = icurr++;
            continue;
        }
        ////Иначе удалить семя
        seeds.erase_after(iprev);
        icurr = std::next(iprev);
    }

    ////Обработка деревьев
    for (auto iTreePrev = trees.before_begin(), iTreeCurr = trees.begin(); iTreeCurr != trees.end();) {
        assert(!iTreeCurr->activeE.empty() || !iTreeCurr->passiveE.empty() || !iTreeCurr->massE.empty());
        ////Если у дерева положительная энергия и допустимый возраст
        if (auto const treeCurrEnergy = iTreeCurr->energy;
                treeCurrEnergy > 0 && iTreeCurr->age-- > 0) {
            auto &activeE = iTreeCurr->activeE;
            for (auto iAEPrev = activeE.before_begin(), iAECurr = std::next(iAEPrev);
                 iAECurr != activeE.end();) {
                auto const activeEBeginConst = iAEPrev;
                bool elemCurrentLock = false, chromoOn = false;
                for (def::Value nChromo{}; nChromo < def::compileConstant.chromoSize; ++nChromo) {
                    assert(!iTreeCurr->genome.empty());
                    def::Value const chromo = iTreeCurr->genome[activeE.front().nChromo][nChromo];
                    if (chromo >= iTreeCurr->genome.size())
                        continue;
                    ////Если хромосома включена
                    chromoOn = true;
                    auto posDisplaced = iAECurr->operator Point().shift(nChromo, 1);
                    auto const border = forest.getBorder();
                    ////Если вышел по горизонту - зациклить
                    if (posDisplaced.x == std::numeric_limits<Point::SizeX_type>::max())
                        posDisplaced.x = border.x - 1;
                    else if (posDisplaced.x >= border.x)
                        posDisplaced.x = 0;
                        ////Если вышел по высоте - вернуть исходную
                    else if (posDisplaced.y >= border.y
                             || posDisplaced.y == std::numeric_limits<Point::SizeY_type>::max())
                        posDisplaced.y = iAECurr->operator Point().y;

                    ////Проверить позицию по номеру хромосомы
                    if (auto[cellPosNew, old] = forest.bind(posDisplaced, &*iTreeCurr, chromo);
                            cellPosNew) {
                        ////Если свободно - залочить и внести в начало активных отростокв
                        activeE.emplace_after(iAEPrev, std::move(cellPosNew.value()));
                        ++iAEPrev;
                    } else if (old->treeBase == &*iTreeCurr)
                        elemCurrentLock = true;
                }
                if (bool lengthChanged = iAEPrev != activeEBeginConst;
                        !chromoOn || lengthChanged) {
                    iTreeCurr->massE.splice_after(iTreeCurr->massE.before_begin(), activeE, iAEPrev);
                    iAECurr = std::next(iAEPrev);
                } else if (elemCurrentLock) {
                    iTreeCurr->passiveE.splice_after(iTreeCurr->passiveE.before_begin(), activeE, iAEPrev);
                    iAECurr = std::next(iAEPrev);
                } else
                    iAEPrev = iAECurr++;
            }

            auto const cellCount = treeElementsPtr.size();
            ////Добавить указатели на все масс клетки дерева в массив для подчёта получаемой энергии
            for (auto &i : iTreeCurr->massE) {
                treeElementsPtr.push_back(&i);
                if (lifeResult) lifeResult->push_back({&*iTreeCurr, i, TypeCell::treeMass, i.nChromo});
            }
            ////Оставшиеся клетки посчитать
            uint16_t cellCountAdd{};
            for (auto &i : iTreeCurr->activeE) {
                ++cellCountAdd;
                if (lifeResult) lifeResult->push_back({&*iTreeCurr, i, TypeCell::treeActive, i.nChromo});
            }
            for (auto &i : iTreeCurr->passiveE) {
                ++cellCountAdd;
                if (lifeResult) lifeResult->push_back({&*iTreeCurr, i, TypeCell::treePassive, i.nChromo});
            }
            ////Вычесть энергию за тик
            iTreeCurr->energy -= (treeElementsPtr.size() - cellCount + cellCountAdd) * lossEnergyCellPerTick;

            if (average)
                *average += Average(iTreeCurr->energy,iTreeCurr->genome.size(),iTreeCurr->age);
            iTreePrev = iTreeCurr++;
        } else {
            ////Смерть дерева
            if (treeCurrEnergy > 0) {
                ////Дерево погибло по истечению срока жизни
                ////Распространить семена из активных и пассивных отростков
                def::Energy energySeed{};
                for (auto &i : iTreeCurr->activeE)
                    ++energySeed;
                for (auto &i : iTreeCurr->passiveE)
                    ++energySeed;
                if (energySeed != 0 && (energySeed = treeCurrEnergy / energySeed) > 0) {
                    for (auto &aLG:iTreeCurr->activeE) {
                        seeds.emplace_front(std::move(aLG), *iTreeCurr, energySeed);
                        if (lifeResult)
                            lifeResult->push_back({&seeds.front(), seeds.front(), TypeCell::seed, 0});
                    }
                    for (auto &aLG:iTreeCurr->passiveE) {
                        seeds.emplace_front(std::move(aLG), *iTreeCurr, energySeed);
                        if (lifeResult)
                            lifeResult->push_back({&seeds.front(), seeds.front(), TypeCell::seed, 0});
                    }
                }
            }
            trees.erase_after(iTreePrev);
            iTreeCurr = std::next(iTreePrev);
        }
    }
    ////Начисление энергии (работа с указателями на локеры элементов деревьев)
    while (!treeElementsPtr.empty()) {
        ////Перемещение в конец массива элементов с координатой X равной последнему
        auto xIt = std::partition(treeElementsPtr.begin(), treeElementsPtr.end(),
                                  [posX = treeElementsPtr.back()->operator Point().x](CellPos const *i) {
                                      return posX != i->operator Point().x;
                                  });
        ////Сортировка по Y по возрастанию
        std::sort(xIt, treeElementsPtr.end(), [](CellPos const *lhs, CellPos const *rhs) {
            return lhs->operator Point().y >= rhs->operator Point().y;
        });
        ////Начисление с учётом коэффициента проникновения света
        int8_t currPower = solidPower, currLevel = solidLevel;
        energyAdd(currPower-- * currLevel--, *(*xIt));
        for (auto i = std::next(xIt); i != treeElementsPtr.end() && currPower > 0; ++i, --currPower) {
            ////Уровень освещённости с учётом пустого пространства между клетками
            currLevel += (*std::prev(i))->operator Point().y - (*i)->operator Point().y - 2;
            ////Не более начального уровня освещённости
            if (currLevel > solidLevel) currLevel = solidLevel;
            if (currLevel > 0)
                energyAdd(currPower * currLevel, *(*i));
        }
        ////Удаление всех указателей на локеры с X координатой
        treeElementsPtr.erase(xIt, treeElementsPtr.end());
    }

    if (average)
        average->endCalculate();
}

void Area::energyAdd(int8_t const e, CellPos &cell) const noexcept {
    if (auto const eAdd = e + e * buffEnergyLimitFromHeight * cell.operator Point().y;
            cell.treeBase->energy + eAdd <
            energyLimit + energyLimit * buffEnergyLimitMaxFromHeight * cell.operator Point().y)
        cell.treeBase->energy += e + eAdd;
}

bool Area::noLife() const noexcept {
    return seeds.empty() && trees.empty();
}

std::ostream &Area::ostream(std::ostream &os, uint8_t mode) const {
    os << TO_LITERAL_STRING(iteration) ": " << iteration << '\n';
    return forest.ostream(os, mode, &trees, mode&Forest::ConsoleViewMode::consoleSeeds ? &seeds : nullptr);
}

namespace simulationTree {
std::ostream &operator<<(std::ostream &os, const simulationTree::Area &area) {
    return area.ostream(os, Forest::ConsoleViewMode::consoleSeeds|Forest::ConsoleViewMode::consoleNChromo);
}

size_t matrixForEach_test(const Area &f, Forest::MatrixForEachPred_test pred) {
    return matrixForEach_test(f.forest, pred);
}

void Area::setConstant(def::Constant const constant) {
    auto const transform = [constant](TreeBase &i) {i.setConstant(constant);};
    std::ranges::for_each(trees, transform);
    std::ranges::for_each(seeds, transform);
}

Area::Average &Area::Average::operator+=(Area::Average const other) noexcept {
    ++totalCount;
    energy += other.energy;
    lenGene += other.lenGene;
    age += other.age;
    return *this;
}

void Area::Average::endCalculate() noexcept {
    energy /= totalCount;
    lenGene /= totalCount;
    age /= totalCount;
}

Area::Average::Average(def::Energy const inE, size_t const inS, def::Value const inA) noexcept
        : energy(inE), lenGene(inS), age(inA) {}
}
