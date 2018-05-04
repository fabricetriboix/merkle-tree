#include "merkle-tree/merkle-tree.hpp"
#include <sstream>
#include <algorithm>

MerkleTree::MerkleTree(const Elements& elements, bool preserveOrder)
    : preserveOrder_(preserveOrder)
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
        if (!preserveOrder_) {
            // Check that this element has not been pushed yet
            if (std::find(elements_.begin(), elements_.end(), *it)
                    != elements_.end()) {
                continue; // ignore duplicates
            }
        }
        elements_.push_back(*it);
    } // for each element

    if (!preserveOrder_) {
        std::sort(elements_.begin(), elements_.end()); // sort elements
    }

    getLayers();
}

MerkleTree::~MerkleTree()
{
}

MerkleTree::Buffer MerkleTree::hash(const Buffer& data)
{
    // TODO blake2b
    Buffer result;
    result.push_back(0);
    for (Buffer::const_iterator it = data.begin(); it < data.end(); ++it) {
        result[0] += *it;
    }
    return result;
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
    while (layers_.back().size() > 1) {
        getNextLayer();
    }
}

void MerkleTree::getNextLayer()
{
    const Elements& previous_layer = layers_.back();

    // Create a new empty layer
    layers_.push_back(Elements());
    Elements& current_layer = layers_.back();

    // For each pair of elements in the previous layer
    // NB: If there is an odd number of elements, we ignore the last one for now
    for (size_t i = 0; i < (previous_layer.size() / 2); i += 2) {
        current_layer.push_back(
                combinedHash(previous_layer[2*i], previous_layer[2*i + 1]));
    }

    // If there is an odd one out at the end, process it
    // NB: It's on its own, so we don't combine it with anything
    if (previous_layer.size() & 1) {
        current_layer.push_back(previous_layer.back());
    }
}

MerkleTree::Buffer MerkleTree::combinedHash(const Buffer& first,
        const Buffer& second) const
{
    Buffer buffer;
    if (preserveOrder_ || (first > second)) {
        std::copy(first.begin(), first.end(), buffer.end());
        std::copy(second.begin(), second.end(), buffer.end());
    } else {
        std::copy(second.begin(), second.end(), buffer.end());
        std::copy(first.begin(), first.end(), buffer.end());
    }
    return hash(buffer);
}
