#include <merkle-tree/merkle-tree.hpp>
#include <gtest/gtest.h>

TEST(MerkleTree, NoElementsShouldThrow)
{
    MerkleTree::Elements empty_elements;
    EXPECT_THROW(MerkleTree m(empty_elements), std::runtime_error);
}
