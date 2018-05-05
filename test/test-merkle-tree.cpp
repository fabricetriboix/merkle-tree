#include <merkle-tree/merkle-tree.hpp>
#include <gtest/gtest.h>

TEST(MerkleTree, NoElementsShouldThrow)
{
    MerkleTree::Elements empty_elements;
    EXPECT_THROW(MerkleTree m(empty_elements), std::runtime_error);
}

TEST(MerkleTree, WrongSizeElementShouldThrow)
{
    MerkleTree::Elements elements;

    MerkleTree::Buffer element;
    for (size_t i = 0; i < MERKLE_TREE_ELEMENT_SIZE; ++i) {
        element.push_back(i);
    }
    elements.push_back(element);

    element.erase(element.begin());
    elements.push_back(element);

    element.push_back(255);
    elements.push_back(element);

    EXPECT_EQ(3u, elements.size());

    EXPECT_THROW(MerkleTree m(elements), std::runtime_error);
}

TEST(MerkleTree, SingleElementShouldBeRoot)
{
    MerkleTree::Buffer data(3, 7); // [7, 7, 7]
    MerkleTree::Buffer element = MerkleTree::hash(data);
    MerkleTree::Elements elements(1, element); // [element]
    EXPECT_EQ(MerkleTree::merkleRoot(elements), element);
}
