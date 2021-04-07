//
// Created by dym on 16.11.2020.
//

#ifndef DEVTREE_TREEBASE_HPP
#define DEVTREE_TREEBASE_HPP

#include <memory_resource>
#include <Definitions.hpp>
#include <Field.hpp>

namespace simulationTree {
namespace def {
using Genome = std::pmr::vector<std::array<Value, compileConstant.chromoSize>>;
using GenomeMonoBuffer =
std::byte[sizeof(Value) * compileConstant.chromoSize * compileConstant.chromoLen.max];
}
class TreeBase {
    def::TreeData treeData;
    def::GenomeMonoBuffer genomeBuffer;
    std::pmr::monotonic_buffer_resource genomeMono;
public:
    def::Genome genome;
    def::Energy energy;
    def::Value age;

    TreeBase() = delete;

    //INIT
    TreeBase(def::TreeDataInit init);

    //MAIN
    TreeBase(TreeBase const &, def::Energy inE)
    #ifndef NDEBUG
    noexcept
    #endif
    ;

    TreeBase(TreeBase &&) noexcept;

    [[nodiscard]] def::TreeData getConstant() const noexcept {return treeData;}

    void setConstant(def::Constant inC);

private:

    TreeBase(TreeBase &&, def::Energy inE) NOEXCEPT_RELEASE;

    TreeBase(decltype(treeData) inTD, decltype(energy) inEenergy) noexcept;

    void correctConstant(def::Constant);
};
}
#endif //DEVTREE_TREEBASE_HPP
