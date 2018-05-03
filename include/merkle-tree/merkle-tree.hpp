#ifndef MERKLE_TREE_HPP_
#define MERKLE_TREE_HPP_

extern "C" {
#include <stdint.h>
}

#include <vector>
#include <list>
#include <stdexcept>

/** Size of a single datum, in bytes */
#define MERKLE_TREE_ELEMENT_SIZE 32

class MerkleTree
{
public :
    /** Buffer type
     *
     * This represents a single datum in the Merkle Tree.
     */
    typedef std::vector<uint8_t> Buffer;

    /** List of elements
     *
     */
    typedef std::list<Buffer> Elements;

    /** Layer
     *
     * TODO
     */
    //typedef TODO Layer;

    /** Constructor
     *
     * \param elements      [in] Elements to add to the Merkle Tree
     * \param preserveOrder [in] Whether to preserve the elements order
     *
     * \throw `std::runtime_error` if `elements` contains elements which are
     *        not of the right size, \see MERKLE_TREE_ELEMENT_SIZE.
     */
    MerkleTree(const Elements& elements, bool preserveOrder = false);

    /** Destructor */
    virtual ~MerkleTree();

    /** Get the root of the Merkle Tree
     *
     * TODO
     */
    //const Layer& getRoot() const;

    // XXX getProof(XXX element, XXX hex) const;

    // XXX getProofOrdered(XXX element, XXX hex) const;

private :
    Elements elements_;
};

// XXX bool checkProofOrdered(proof, root, element, index);
// XXX bool checkProof(proof, root, element);
// XXX merkleRoot(elements, preserveOrder);

#endif // MERKLE_TREE_HPP_
