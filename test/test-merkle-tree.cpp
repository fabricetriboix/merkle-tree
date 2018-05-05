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

TEST(MerkleTree, DuplicatesShouldBeRemovedByDefault)
{
    MerkleTree::Buffer data0(3, 5);
    MerkleTree::Buffer data1(6, 9);
    MerkleTree::Buffer hash0 = MerkleTree::hash(data0);
    MerkleTree::Buffer hash1 = MerkleTree::hash(data1);

    MerkleTree::Elements elements0;
    elements0.push_back(hash0);
    elements0.push_back(hash0);
    EXPECT_EQ(MerkleTree::merkleRoot(elements0), hash0);

    MerkleTree::Elements elements1;
    elements1.push_back(hash0);
    elements1.push_back(hash1);
    MerkleTree::Elements elements2;
    elements2.push_back(hash0);
    elements2.push_back(hash1);
    elements2.push_back(hash0);
    EXPECT_EQ(MerkleTree::merkleRoot(elements1),
            MerkleTree::merkleRoot(elements2));
}

TEST(MerkleTree, CheckProofWithOneElement)
{
    MerkleTree::Buffer data0(7, 3);
    MerkleTree::Buffer hash0 = MerkleTree::hash(data0);
    MerkleTree::Elements elements(1, hash0);
    MerkleTree merkle_tree(elements);

    MerkleTree::Elements proof = merkle_tree.getProof(hash0);
    EXPECT_EQ(0, proof.size());

    MerkleTree::Buffer root = merkle_tree.getRoot();
    EXPECT_EQ(root, hash0);

    EXPECT_TRUE(MerkleTree::checkProof(proof, root, hash0));
}
