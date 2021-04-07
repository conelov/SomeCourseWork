//
// Created by dym on 02.12.2020.
//

#include "Tree.hpp"

using namespace simulationTree;

Tree::Tree(Seed &&inS) noexcept
        : TreeBase(std::move(inS)),
          mono(buffer, sizeof(buffer)),
          activeE(&mono), passiveE(&mono), massE(&mono) {
    assert(inS.operator Point().y == 0);
    inS.treeBase = this;
    activeE.emplace_front(std::move(inS));
}

//#ifndef NDEBUG
//
//    #include <algorithm>
//    #include <string>
//
//Tree::~Tree() noexcept(false) {
//    constexpr auto each = [](Cell const &i) {return i.operator bool();};
//    if (energy > 0)
//        if (auto const countNotProcessing =
//                    std::ranges::count_if(activeE, each) +
//                    std::ranges::count_if(passiveE, each);
//                countNotProcessing != 0) {
//            using namespace std::literals;
//            throw std::logic_error("Tree:~Tree(): not all elements are processed. Count elem: "s +
//                                     std::to_string(countNotProcessing));
//        }
//}
//
//#endif