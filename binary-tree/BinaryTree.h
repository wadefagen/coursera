/**
 * BinaryTree class outline.
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>, Eric Huber
 */

#pragma once

template <typename T>
class BinaryTree {
  public:
    // ...

  private:
    class TreeNode {
      public:
        // *See note below about how references are being used here.
        T & data;
        TreeNode *left, *right;
        TreeNode(T & data) : data(data), left(nullptr), right(nullptr) { }
    };

    TreeNode *root_;
};

// *Note that this implementation of a tree is storing explicit aliases
// to memory using references. This means that the actual allocation,
// cleanup, and general responsibility for that data memory will lie
// outside of this class implementation. However, the tree nodes
// themselves belong to the tree and will need to be created or
// deleted by the tree.

// Sometimes, your header files might include another header file with
// further templated definitions. The .h and .hpp are both just filename
// extensions for header files.
#include "BinaryTree.hpp"

