#include <gtest/gtest.h>
#include <Definitions.hpp>

using namespace simulationTree;

#include <Field.hpp>

namespace simulationTree {
class Area;

bool matrixForEachPredDefault_test(Matrix::value_type const i) {
    return i != nullptr;
}

size_t matrixForEach_test(Forest const &f,
                          Forest::MatrixForEachPred_test pred = matrixForEachPredDefault_test);

size_t matrixForEach_test(Area const &f, Forest::MatrixForEachPred_test pred = matrixForEachPredDefault_test);

}

#include "TreeBase.hpp"

TEST(Constant, _1) {
    def::ConstantInit ci{};
    def::Constant c{ci.operator def::Constant()};
    ASSERT_LE(c.chromoNewLenChance, ci.chromoNewLenChance.max);
    ASSERT_GE(c.chromoNewLenChance, ci.chromoNewLenChance.min);
    ASSERT_LE(c.chromoMutatChance, ci.chromoMutatChance.max);
    ASSERT_GE(c.chromoNewLenChance, ci.chromoMutatChance.min);
    ASSERT_LE(c.chromoMutatTrueChance, ci.chromoMutatTrueChance.max);
    ASSERT_GE(c.chromoNewLenChance, ci.chromoMutatTrueChance.min);
    ASSERT_EQ(c.ageInit, ci.age);
}

TEST(TreeBaseTest, _1) {
    def::ConstantInit ci{};
    def::TreeDataInit tdi{{}, ci};
    TreeBase tb1(tdi);
    ASSERT_LE(tb1.genome.size(), ci.chromoLen.max);
    ASSERT_GE(tb1.genome.size(), ci.chromoLen.min);

    def::Constant c = ci;
    c.chromoMutatChance = def::compileConstant.valueOff;
    c.chromoNewLenChance = 1;
    tb1.setConstant(c);
    TreeBase tb2(tb1, 90);
    ASSERT_EQ(tb1.genome.size() + 1, tb2.genome.size());

    c.chromoNewLenChance = 0;
    tb1.setConstant(c);
    tb2.~TreeBase();
    new(&tb2) TreeBase(tb1, 90);
    ASSERT_EQ(tb1.genome.size() - 1, tb2.genome.size());

    c.chromoMutatChance = c.chromoNewLenChance = def::compileConstant.valueOff;
    tb1.setConstant(c);
    tb2.~TreeBase();
    new(&tb2) TreeBase(tb1, 90);
    ASSERT_EQ(tb1.genome, tb2.genome);

    c.chromoMutatChance = 1;
    c.chromoMutatTrueChance = 1;
    tb1.setConstant(c);
    tb2.~TreeBase();
    new(&tb2) TreeBase(tb1, 90);
    ASSERT_EQ(tb1.genome.size(), tb2.genome.size());
    ASSERT_GE(1, [&]() {
        size_t count{};
        for (size_t i{}; i < tb1.genome.size(); ++i)
            if (tb1.genome.at(i) != tb2.genome.at(i))
                for (size_t j{}; j < def::compileConstant.chromoSize; ++j)
                    if (tb1.genome.at(i)[j] != tb2.genome.at(i)[j]
                        && tb2.genome.at(i)[j] < tb2.genome.size())
                        ++count;
        return count;
    }());

    c.chromoMutatTrueChance = def::compileConstant.valueOff;
    tb1.setConstant(c);
    tb2.~TreeBase();
    new(&tb2) TreeBase(tb1, 90);
    ASSERT_EQ(tb1.genome.size(), tb2.genome.size());
    ASSERT_GE(1, [&]() {
        size_t count{};
        for (size_t i{}; i < tb1.genome.size(); ++i)
            if (tb1.genome.at(i) != tb2.genome.at(i))
                for (size_t j{}; j < def::compileConstant.chromoSize; ++j)
                    if (tb1.genome.at(i)[j] != tb2.genome.at(i)[j]
                        && tb2.genome.at(i)[j] == def::compileConstant.valueOff)
                        ++count;
        return count;
    }());
}

TEST(ForestTest, _1) {
    Forest forest({5, 5});

    ASSERT_FALSE(forest.observe({0, 0}));
    ASSERT_EQ(0, matrixForEach_test(forest));

    auto b1 = forest.bind({0, 0});
    ASSERT_TRUE(b1.first);
    ASSERT_TRUE(b1.first->operator bool());
    ASSERT_FALSE(b1.second);
    ASSERT_EQ(1, matrixForEach_test(forest));

    auto b2 = forest.bind({0, 0});
    ASSERT_FALSE(b2.first);
    ASSERT_TRUE(b2.second);
    ASSERT_TRUE(b2.second->operator bool());
    ASSERT_EQ(b2.second, &b1.first.value());
    ASSERT_EQ(1, matrixForEach_test(forest));

    b2.first.reset();
    ASSERT_TRUE(b2.second);
    ASSERT_TRUE(b2.second->operator bool());
    ASSERT_EQ(1, matrixForEach_test(forest));

    b1.first.reset();
    auto b3 = forest.bind({0, 0});
    ASSERT_TRUE(b3.first);
    ASSERT_TRUE(b3.first->operator bool());
    ASSERT_FALSE(b3.second);
    ASSERT_EQ(1, matrixForEach_test(forest));

    ASSERT_EQ(nullptr, forest.rebind(b3.first.value(), {0, 1}));
    ASSERT_TRUE(b3.first->operator bool());
    ASSERT_EQ(1, matrixForEach_test(forest));

    auto b4 = forest.bind({0, 0});
    ASSERT_EQ(&b3.first.value(), forest.rebind(b4.first.value(), {0, 1}));

    b4.first.reset();
    b3.first.reset();
    ASSERT_EQ(0, matrixForEach_test(forest));
}

#include "Seed.hpp"

TEST(SeedTest, _1) {
    Forest forest({5, 5});

    Seed s1(forest.bind({0, 0}), def::TreeDataInit{});
    ASSERT_EQ(forest.observe({0, 0}), &s1);
    ASSERT_EQ(1, matrixForEach_test(forest));
}

#include "Tree.hpp"

TEST(TreeTest, _1) {
    Forest forest({5, 5});
    def::TreeDataInit treeDataInit;

    Tree t1(Seed(forest.bind({0, 0}), treeDataInit));
    ASSERT_EQ(1, matrixForEach_test(forest));
    ASSERT_THROW(t1.~Tree(), std::runtime_error);

    new(&t1) Tree(Seed(forest.bind({0, 0}), treeDataInit));
    t1.massE.splice_after(t1.massE.cbefore_begin(), t1.activeE, t1.activeE.cbefore_begin());
}

#include "Area.hpp"
#include <ch_FPSControl.h>

TEST(AreaTest, _1) {
    constexpr uint16_t amountTree = 100;
    Area area({100, 50}, amountTree, {}, {49, 0});
    ASSERT_EQ(amountTree, matrixForEach_test(area));
    ch::Timer timer;
    while (!area.noLife() && timer.elapsed() < 3)
        area.life();
    std::cout << "elapsed: " << timer.elapsed() << "\t\titeration: " << area.iterations() << std::endl;
}
