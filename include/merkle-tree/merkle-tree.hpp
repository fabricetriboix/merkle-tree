#ifndef MERKLE_TREE_HPP_
#define MERKLE_TREE_HPP_

extern "C" {
#include <stdint.h>
}

#include <vector>
#include <deque>
#include <stdexcept>

/** Size of a hash, in bytes
 *
 * We are using Blake2b with an output of 128 bits, which is 16 bytes.
 */
#define MERKLE_TREE_ELEMENT_SIZE 16

class MerkleTree
{
public :
    /** Buffer type
     *
     * This represents a single hash in the Merkle Tree and must have a
     * length of `MERKLE_TREE_ELEMENT_SIZE`.
     *
     * \see MERKLE_TREE_ELEMENT_SIZE
     */
    typedef std::vector<uint8_t> Buffer;

    /** List of elements
     *
     * This is a list of hashes.
     */
    typedef std::deque<Buffer> Elements;

    /** Constructor
     *
     * \param elements      [in] Elements to add to the Merkle Tree
     *                           There must be at least one element
     * \param preserveOrder [in] Whether to preserve the elements order
     *
     * \throw `std::runtime_error` if `elements` contains elements which are
     *        not of the right size, \see MERKLE_TREE_ELEMENT_SIZE.
     *
     * \throw `std::runtime_error` if `elements` is empty
     */
    MerkleTree(const Elements& elements, bool preserveOrder = false);

    /** Destructor */
    virtual ~MerkleTree();

    /** Compute a hash
     *
     * \param data [in] Data to hash
     *
     * \return The computed hash of `data`
     */
    static Buffer hash(const Buffer& data);

    /** Get the root of the Merkle Tree
     *
     * TODO
     */
    //const Layer& getRoot() const;

    // XXX getProof(XXX element, XXX hex) const;

    // XXX getProofOrdered(XXX element, XXX hex) const;

private :
    /** Layers data structure
     *
     * This data structure represents the various layers of the Merkle Tree.
     * The first layer is the initial list of hashes, the 2nd layer is the
     * combination of the hashes of the first layer, etc. until the last layer
     * which is the top-level hash, aka the root. The last layer has a length
     * of one.
     */
    typedef std::deque<Elements> Layers;

    bool     preserveOrder_; /**< Whether to preserver the initial order */
    Elements elements_;      /**< Leaves of the Merkle Tree */
    Layers   layers_;        /**< The actual Merkle Tree */

    /** Build the Merkle Tree layers */
    void getLayers();

    /** Build the next Merkle Tree layer */
    void getNextLayer();

    /** Combine two hashes into one */
    Buffer combinedHash(const Buffer& first, const Buffer& second) const;
};

// XXX bool checkProofOrdered(proof, root, element, index);
// XXX bool checkProof(proof, root, element);
// XXX merkleRoot(elements, preserveOrder);

#endif // MERKLE_TREE_HPP_
