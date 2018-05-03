#include "merkle-tree/merkle-tree.hpp"
#include <sstream>

MerkleTree::MerkleTree(const std::list<Buffer>& elements, bool preserveOrder)
{
    for (   Elements::const_iterator it = elements.begin();
            it != elements.end();
            ++it) {
        if (it->empty()) {
            continue; // ignore empty elements
        }
        if (it->size() != MERKLE_TREE_ELEMENT_SIZE) {
            std::ostringstream oss;
            oss << "Element size is " << it->size() << ", it must be "
                << MERKLE_TREE_ELEMENT_SIZE;
            throw std::runtime_error(oss.str());
        }
    } // for each element

    if (!preserveOrder) {
        elements_.unique(); // remove duplicates
        elements_.sort(); // sort elements
    }

    // layers_ = XXX;
}

MerkleTree::~MerkleTree()
{
}
