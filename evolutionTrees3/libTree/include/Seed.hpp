//
// Created by dym on 02.12.2020.
//

#ifndef DEVTREE_SEED_HPP
#define DEVTREE_SEED_HPP

#include "Field.hpp"
#include "TreeBase.hpp"

namespace simulationTree {
struct Seed : public TreeBase, public CellPos {

    template<typename ...Args>
    explicit Seed(CellPos &&inCP, Args &&...args) noexcept
            : TreeBase(std::forward<Args>(args)...),
              CellPos(std::move(inCP)) {
        treeBase = this;
        nChromo = def::compileConstant.initialNChromo;
    }

    template<typename ...Args>
    explicit Seed(Forest::BindResult::first_type &&inCP, Args &&...args) noexcept
            :Seed(std::move(*inCP), std::forward<Args>(args)...) {}

    template<typename ...Args>
    explicit Seed(Forest::BindResult &&inCP, Args &&...args) noexcept
            :Seed(std::move(inCP.first), std::forward<Args>(args)...) {}
};
}
#endif //DEVTREE_SEED_HPP
