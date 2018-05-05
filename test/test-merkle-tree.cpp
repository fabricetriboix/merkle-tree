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

TEST(MerkleTree, CheckProofWithTwoElements)
{
    MerkleTree::Buffer data0(78, 4);
    MerkleTree::Buffer hash0 = MerkleTree::hash(data0);
    MerkleTree::Buffer data1(4, 89);
    MerkleTree::Buffer hash1 = MerkleTree::hash(data1);

    MerkleTree::Elements elements;
    elements.push_back(hash0);
    elements.push_back(hash1);
    MerkleTree merkle_tree(elements);

    MerkleTree::Elements proof0 = merkle_tree.getProof(hash0);
    MerkleTree::Buffer root = merkle_tree.getRoot();
    ASSERT_EQ(1u, proof0.size());
    EXPECT_EQ(hash1, proof0[0]);
    EXPECT_EQ(root, MerkleTree::combinedHash(hash0, hash1, false));
    EXPECT_TRUE(MerkleTree::checkProof(proof0, root, hash0));

    MerkleTree::Elements proof1 = merkle_tree.getProof(hash1);
    ASSERT_EQ(1u, proof1.size());
    EXPECT_EQ(hash0, proof1[0]);
    EXPECT_TRUE(MerkleTree::checkProof(proof1, root, hash1));
}

TEST(MerkleTree, CheckProofWithThreeElements)
{
    MerkleTree::Buffer data0(34, 45);
    MerkleTree::Buffer data1(435, 5);
    MerkleTree::Buffer data2(4, 5);
    MerkleTree::Buffer hash0 = MerkleTree::hash(data0);
    MerkleTree::Buffer hash1 = MerkleTree::hash(data1);
    MerkleTree::Buffer hash2 = MerkleTree::hash(data2);
    MerkleTree::Buffer calculated_hash01 = MerkleTree::combinedHash(hash0,
            hash1, true);
    MerkleTree::Buffer calculated_root = MerkleTree::combinedHash(
            calculated_hash01, hash2, true);

    MerkleTree::Elements elements;
    elements.push_back(hash0);
    elements.push_back(hash1);
    elements.push_back(hash2);
    MerkleTree merkle_tree(elements, true);

    MerkleTree::Elements proof0 = merkle_tree.getProof(hash0);
    ASSERT_EQ(2u, proof0.size());
    if (proof0[0] == hash1) {
        EXPECT_EQ(proof0[1], hash2);
    } else {
        EXPECT_EQ(proof0[0], hash2);
        EXPECT_EQ(proof0[1], hash1);
    }

    MerkleTree::Buffer root = merkle_tree.getRoot();
    EXPECT_EQ(root, calculated_root);

    MerkleTree::Elements proof1 = merkle_tree.getProof(hash1);
    ASSERT_EQ(2u, proof1.size());
    if (proof1[0] == hash0) {
        EXPECT_EQ(proof1[1], hash2);
    } else {
        EXPECT_EQ(proof1[0], hash2);
        EXPECT_EQ(proof1[1], hash0);
    }
    EXPECT_TRUE(MerkleTree::checkProof(proof1, root, hash1));

    MerkleTree::Elements proof2 = merkle_tree.getProof(hash2);
    ASSERT_EQ(1u, proof2.size());
    EXPECT_EQ(calculated_hash01, proof2[0]);
    EXPECT_TRUE(MerkleTree::checkProof(proof2, root, hash2));
}
