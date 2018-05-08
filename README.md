Merkle Tree C++ implementation
==============================

Build
-----

The build system is based on cmake, version at least 3.1; so you
need to install that on your system

To build on Linux:

```sh
$ git clone --recursive https://github.com/fabricetriboix/merkle-tree.git
$ mkdir build
$ cd build
$ cmake -DCMAKE_BUILD_TYPE=Release ../merkle-tree
$ make
$ make doc
$ make test
```

Usage
-----

To build a Merkle Tree, you need to pass it a list (actually a deque) of
hashes (also called elements in this implementation). You can use the
static function `MerkleTree::hash()` to compute hashes of any buffer of
bytes.

You can choose to have the Merkle Tree in the order you specified (when
the `preserveOrder` constructor argument is set to `true`), or to have
the elements sorted and duplicates removed (when the `preserveOrder`
constructor argument is set to `false`).

After that, you can get a proof for any elements that has been used to
build the Merkle Tree using `getProof()` or `getProofOrdered()` (the
latter should be used when the tree has been built with
`preserveOrder` set to `true`).

A proof can be checked using the static functions
`MerkleTree::checkProof()` and `MerkleTree::checkProofOrdered()`
(again, the latter should be used when the tree has been built with
`preserveOrder` set to `true`).

Please refer to the doxygen-generated documentation for more details,
or the `test/test-merkle-tree.cpp` test file for examples.

Credits
-------

Merkle tree source code ported from
[merkle-tree-solidity](https://github.com/ameensol/merkle-tree-solidity).

I tried to keep to original names wherever possible.

Blake2b implementation from
[BLAKE2 reference code](https://github.com/BLAKE2/BLAKE2).

This code is license according to any working combination of the above.
