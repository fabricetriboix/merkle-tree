#include <merkle-tree/merkle-tree.hpp>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::UnorderedElementsAre;

TEST(MerkleTreeUnordered, NoElementsShouldThrow)
{
    MerkleTree::Elements empty_elements;
    EXPECT_THROW(MerkleTree m(empty_elements), std::runtime_error);
}

TEST(MerkleTreeUnordered, WrongSizeElementShouldThrow)
{
    MerkleTree::Elements elements;

    MerkleTree::Buffer element;
    for (size_t i = 0; i < MERKLE_TREE_ELEMENT_SIZE_B; ++i) {
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

TEST(MerkleTreeUnordered, SingleElementShouldBeRoot)
{
    MerkleTree::Buffer data(3, 7); // [7, 7, 7]
    MerkleTree::Buffer element = MerkleTree::hash(data);
    MerkleTree::Elements elements(1, element); // [element]
    EXPECT_EQ(MerkleTree::merkleRoot(elements), element);
}

TEST(MerkleTreeUnordered, DuplicatesShouldBeRemovedByDefault)
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

TEST(MerkleTreeUnordered, CheckProofWithOneElement)
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

TEST(MerkleTreeUnordered, CheckProofWithTwoElements)
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

TEST(MerkleTreeUnordered, CheckProofWithTenElements)
{
    MerkleTree::Elements elements;
    for (size_t i = 0; i < 10; ++i) {
        MerkleTree::Buffer data(1, i);
        elements.push_back(MerkleTree::hash(data));
    }
    MerkleTree merkle_tree(elements);
    MerkleTree::Buffer root = merkle_tree.getRoot();
    for (   MerkleTree::Elements::iterator it = elements.begin();
            it != elements.end();
            ++it) {
        MerkleTree::Elements proof = merkle_tree.getProof(*it);
        EXPECT_TRUE(MerkleTree::checkProof(proof, root, *it));
    }

    std::reverse(elements.begin(), elements.end());
    MerkleTree reverse_tree(elements);
    EXPECT_EQ(root, reverse_tree.getRoot());
}

TEST(MerkleTreeOrdered, CheckProofWithTwoElements)
{
    MerkleTree::Buffer data0(45, 37);
    MerkleTree::Buffer data1(5, 7);
    MerkleTree::Buffer hash0 = MerkleTree::hash(data0);
    MerkleTree::Buffer hash1 = MerkleTree::hash(data1);

    MerkleTree::Elements elements;
    elements.push_back(hash0);
    elements.push_back(hash1);
    MerkleTree ordered_tree(elements, true);

    MerkleTree::Elements proof0 = ordered_tree.getProof(hash0);
    ASSERT_EQ(1u, proof0.size());
    EXPECT_EQ(hash1, proof0[0]);

    MerkleTree::Buffer root = ordered_tree.getRoot();
    MerkleTree::Buffer calculcated_root = MerkleTree::combinedHash(
            hash0, hash1, true);
    EXPECT_EQ(root, calculcated_root);
    EXPECT_TRUE(MerkleTree::checkProofOrdered(proof0, root, hash0, 1));

    MerkleTree::Elements proof1 = ordered_tree.getProof(hash1);
    ASSERT_EQ(1u, proof1.size());
    EXPECT_EQ(hash0, proof1[0]);
    EXPECT_TRUE(MerkleTree::checkProofOrdered(proof1, root, hash1, 2));
}

TEST(MerkleTreeOrdered, CheckProofWithThreeElements)
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
    MerkleTree ordered_tree(elements, true);

    MerkleTree::Elements proof0 = ordered_tree.getProof(hash0);
    ASSERT_EQ(2u, proof0.size());
    EXPECT_THAT(proof0, UnorderedElementsAre(hash1, hash2));

    MerkleTree::Buffer root = ordered_tree.getRoot();
    EXPECT_EQ(root, calculated_root);

    MerkleTree::Elements proof1 = ordered_tree.getProof(hash1);
    ASSERT_EQ(2u, proof1.size());
    EXPECT_THAT(proof1, UnorderedElementsAre(hash0, hash2));
    EXPECT_TRUE(MerkleTree::checkProofOrdered(proof1, root, hash1, 2));

    MerkleTree::Elements proof2 = ordered_tree.getProof(hash2);
    ASSERT_EQ(1u, proof2.size());
    EXPECT_EQ(calculated_hash01, proof2[0]);
    EXPECT_TRUE(MerkleTree::checkProofOrdered(proof2, root, hash2, 3));
}

TEST(MerkleTreeOrdered, DuplicatesShouldBePreserved)
{
    MerkleTree::Buffer data0(34, 45);
    MerkleTree::Buffer data1(435, 5);
    MerkleTree::Buffer data2(34, 45);
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
    MerkleTree ordered_tree(elements, true);

    MerkleTree::Elements proof0 = ordered_tree.getProofOrdered(hash0, 1);
    ASSERT_EQ(2u, proof0.size());
    EXPECT_THAT(proof0, UnorderedElementsAre(hash1, hash2));

    MerkleTree::Buffer root = ordered_tree.getRoot();
    EXPECT_EQ(root, calculated_root);

    MerkleTree::Elements proof1 = ordered_tree.getProofOrdered(hash1, 2);
    ASSERT_EQ(2u, proof1.size());
    EXPECT_THAT(proof1, UnorderedElementsAre(hash0, hash2));
    EXPECT_TRUE(MerkleTree::checkProofOrdered(proof1, root, hash1, 2));

    MerkleTree::Elements proof2 = ordered_tree.getProofOrdered(hash2, 3);
    ASSERT_EQ(1u, proof2.size());
    EXPECT_EQ(calculated_hash01, proof2[0]);
    EXPECT_TRUE(MerkleTree::checkProofOrdered(proof2, root, hash2, 3));
}

TEST(MerkleTreeOrdered, CheckProofWithTenElements)
{
    MerkleTree::Elements elements;
    for (size_t i = 0; i < 10; ++i) {
        MerkleTree::Buffer data(1, i);
        elements.push_back(MerkleTree::hash(data));
    }
    MerkleTree ordered_tree(elements, true);
    MerkleTree::Buffer root = ordered_tree.getRoot();
    for (size_t i = 0; i < elements.size(); ++i) {
        MerkleTree::Elements proof = ordered_tree.getProofOrdered(elements[i], i+1);
        EXPECT_TRUE(MerkleTree::checkProofOrdered(proof, root, elements[i], i+1));
    }
}

TEST(MerkleTreeOrdered, CheckProofWithTenElementsWithDuplicates)
{
    MerkleTree::Elements elements;
    for (size_t i = 0; i < 10; ++i) {
        MerkleTree::Buffer data(1, i % 5);
        elements.push_back(MerkleTree::hash(data));
    }
    MerkleTree ordered_tree(elements, true);
    MerkleTree::Buffer root = ordered_tree.getRoot();
    for (size_t i = 0; i < elements.size(); ++i) {
        MerkleTree::Elements proof = ordered_tree.getProofOrdered(elements[i], i+1);
        EXPECT_TRUE(MerkleTree::checkProofOrdered(proof, root, elements[i], i+1));
    }
}
