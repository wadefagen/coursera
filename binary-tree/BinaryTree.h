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
        // Note that you can declare multiple pointers on the same line as
        // shorthand, like this:
        //   TreeNode *left, *right;
        // But since this requires you to write the "*" with each variable
        // name, it can be a little confusing, or prone to making a mistake.
        // Instead, you can declare the pointers on separate lines like this:
        TreeNode* left;
        TreeNode* right;
        // **See note below about how this initialization list is styled.
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

// **This TreeNode constructor initializes its own "data" member using
// the argument also called "data". This is one of VERY FEW places in
// C++ where you can reuse the same variable name to mean different
// things at the same time.
//   Within the initialization list only, when you write data(data) as
// shown, it correctly initializes the member variable called
// "data" using the argument called "data". But if you write "data"
// again in the function body, then you will be referring to the
// function argument, not to the member variable. It's best to avoid
// styling your code like this to avoid making mistakes. Just give
// the function parameter a different name, such as "dataArgument".
// See the binary-tree-traversals example directory for a different
// version, where we show this.

// Sometimes, your header files might include another header file with
// further templated definitions. The .h and .hpp are both just filename
// extensions for header files.
#include "BinaryTree.hpp"

