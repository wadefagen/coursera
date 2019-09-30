/**
 * Dictionary class outline for BST (binary search tree) example.
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>, Eric Huber
 */

// This is a BST-based implementation of a Dictionary ADT as described in
// lecture. Remember that a dictionary ADT describes the user interface for
// your class, and the underlying data structure can be implemented with
// things besides a BST. For example, later in the course sequence you will
// deal with other implementations of a dictionary ADT based on hash tables.

#pragma once

#include <stdexcept>

template <typename K, typename D>
class Dictionary {
  public:
    Dictionary();
    const D& find(const K& key);
    void insert(const K& key, const D& data);
    const D& remove(const K& key);

  private:
    class TreeNode {
      public:
        // *See note below about how references are being used here.
        const K& key;
        const D& data;
        TreeNode *left, *right;
        // **See note below about how this initialization list is styled.
        TreeNode(const K& key, const D& data)
          : key(key), data(data), left(nullptr), right(nullptr) { }
    };

    TreeNode *head_;

    TreeNode*& _find(const K& key, TreeNode*& cur) const;

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
// shown below, it correctly initializes the member variable called
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
#include "Dictionary.hpp"

