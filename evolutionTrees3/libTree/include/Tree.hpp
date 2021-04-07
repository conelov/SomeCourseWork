//
// Created by dym on 02.12.2020.
//

#ifndef DEVTREE_TREE_HPP
#define DEVTREE_TREE_HPP

#include "Seed.hpp"

namespace simulationTree {

class Tree : public TreeBase {
    struct CellPosNodeSizeof {
        CellPos cellPos;
        void *ptr;
    };
    std::byte buffer[sizeof(CellPosNodeSizeof) * 20];
    std::pmr::monotonic_buffer_resource mono;
public:
    std::pmr::forward_list<CellPos> activeE, passiveE, massE;

    explicit Tree(Seed &&inS) noexcept;

//    #ifndef NDEBUG
//    ~Tree() noexcept(false);
//    #endif
};
}
#endif //DEVTREE_TREE_HPP
