#include "merkle-tree/merkle-tree.hpp"
#include <sstream>
#include <algorithm>

MerkleTree::MerkleTree(const Elements& elements, bool preserveOrder)
{
    if (elements.empty()) {
        throw std::runtime_error("Empty elements list");
    }

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
        if (!preserveOrder) {
            // Check that this element has not been pushed yet
            if (std::find(elements_.begin(), elements_.end(), *it)
                    != elements_.end()) {
                continue; // ignore duplicates
            }
        }
        elements_.push_back(*it);
    } // for each element

    if (!preserveOrder) {
        std::sort(elements_.begin(), elements_.end()); // sort elements
    }

    getLayers();
}

MerkleTree::~MerkleTree()
{
}

void MerkleTree::getLayers()
{
    layers_.clear();

    // The first layer is the elements themselves
    layers_.push_back(elements_);

    if (elements_.empty()) {
        return; // nothing left to do
    }

    // For subsequent layers, combine each pair of hashes in the previous
    // layer to build the current layer. Repeat until the current layer has
    // only one hash (this will be the root of the tree).
//    while (layers_.back().size() > 1) {
//        getNextLayer();
//    }
}

//void MerkleTree::getNextLayer()
//{
//    const Elements& current_layer = layers_.back();
//}
