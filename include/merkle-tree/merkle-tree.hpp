#ifndef MERKLE_TREE_HPP_
#define MERKLE_TREE_HPP_

extern "C" {
#include <stdint.h>
}

#include <vector>
#include <list>

class MerkleTree
{
public :
    /** Buffer type
     *
     * This represents a single datum in the Merkle Tree.
     */
    typedef std::vector<uint8_t> Buffer;

    /** Layer
     *
     * TODO
     */
    //typedef TODO Layer;

    /** Constructor
     *
     */
    MerkleTree(const std::list<Buffer>& elements, bool preserveOrder = false);

    /** Destructor */
    virtual ~MerkleTree();

    /** Get the root of the Merkle Tree
     *
     * TODO
     */
    //const Layer& getRoot() const;

private :
    int dummy_;
};

#endif // MERKLE_TREE_HPP_
