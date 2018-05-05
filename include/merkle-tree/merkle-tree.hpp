#ifndef MERKLE_TREE_HPP_
#define MERKLE_TREE_HPP_

extern "C" {
#include <stdint.h>
}

#include <vector>
#include <deque>
#include <string>
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

    /** TODO
     *
     * \param index [in] Starts a 1, not 0
     */
    static bool checkProofOrdered(const Elements& proof, const Buffer& root,
            const Buffer& element, size_t index);

    /** Get the root hash of the Merkle Tree */
    Buffer getRoot() const
    {
        return layers_.back()[0];
    }

    /** Get proof
     *
     * This function returns a list of hashes, starting from the hash of the
     * paired element of `element`, up to the top-level hash.
     *
     * \param element [in] Element to get the proof for
     *
     * \throw `std::runtime_error` if `element` is not in the base layer of
     *        the Merkle Tree
     */
    Elements getProof(const Buffer& element) const;

    /** Get proof in string form
     *
     */
    std::string getProofHex(const Buffer& element) const;

    /** Get proof ordered
     *
     * \param element [in] Element to get the proof for
     * \param index   [in] Index of above element
     *
     * **IMPORTANT NOTE**: `index` starts at 1, not at 0; so the first element
     *                     has an index of 1, the second element an index of
     *                     2, etc.
     *
     * \throw `std::runtime_error` if `index` does not point to `element`
     */
    Elements getProofOrdered(const Buffer& element, size_t index) const;

    /** Get proof ordered in string form
     *
     * \param element [in] Element to get the proof for
     * \param index   [in] Index of above element
     *
     * **IMPORTANT NOTE**: `index` starts at 1, not at 0; so the first element
     *                     has an index of 1, the second element an index of
     *                     2, etc.
     *
     * \throw `std::runtime_error` if `index` does not point to `element`
     */
    std::string getProofOrderedHex(const Buffer& element, size_t index) const;

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

    static Buffer combinedHash(const Buffer& first, const Buffer& second,
            bool preserveOrder_);

    /** Get proof given the index of the element
     *
     */
    Elements getProof(size_t index) const;

    /** Get the peer of an element
     *
     * \param layer [in]  Layer to search
     * \param index [in]  Index of element in layer
     * \param pair  [out] Peer element
     *
     * \return `true` if OK, `false` if no peer element (this can happen if
     *         the `layer` has an odd number of elements, and you are asking
     *         for the last one, which obviously has no peer)
     */
    static bool getPair(const Elements& layer, size_t index, Buffer& pair);
};

// XXX bool checkProof(proof, root, element);
// XXX merkleRoot(elements, preserveOrder);

#endif // MERKLE_TREE_HPP_
