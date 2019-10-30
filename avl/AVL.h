/**
 * AVL class outline
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>, Eric Huber
 */

// Important notes from the course staff:

// Before studying this example, please first study these other example
// directories:
//   binary-tree
//   binary-tree-traversals
//   bst
// These contain notes that are useful prerequisite knowledge for better
// understanding how this AVL example is implemented. In particular, this
// example was directly based on the code in the bst directory, and many
// of the specific code comments in that project also apply to this example.
// (Anything that appears to lack comments in this AVL example was probably
// already explained in the other examples. There may be some comments in
// this example that are nearly the same as those in the BST example.)

// This example is based on original work by Prof. Wade Fagen-Ulmschneider as
// shown in lecture. The course staff have edited it slightly and added code
// comments, so the line numbering will not match up exactly. Some repeated
// chunks of code were refactored into small helper functions, such as
// get_balance_factor, to better check for correctness. There are also some
// additional lines of code here and there for error checking. -Eric

#pragma once

// We include <stdexcept> so we can throw std::runtime_error in some cases.
#include <stdexcept>
// We include <utility> for the std::swap function
#include <utility>
// We'll add a "printInOrder" function to help us inspect the results.
// This will require std::cout from <iostream>.
#include <iostream>
// We include <algorithm> for std::max
#include <algorithm>

template <typename K, typename D>
class AVL {
  public:
    // Let the constructor just initialize the head pointer to null.
    // We'll skip implementing other constructors for this example.
    AVL() : head_(nullptr) { }

    // find, insert, remove: Please see AVL.hpp for comments on these.
    const D& find(const K& key);
    void insert(const K& key, const D& data);
    const D& remove(const K& key);

  private:
    class TreeNode {
      public:
        // *See note 1 below about how references are being used here.
        const K& key;
        const D& data;
        TreeNode *left, *right;
        int height;
        // **See note 2 below about how this initialization list is styled.
        TreeNode(const K& key, const D& data)
          : key(key), data(data), left(nullptr), right(nullptr), height(0) { }
    };

    TreeNode *head_;

    // These internal helper functions are private because they are only
    // meant to be used by other member functions of our class. Please see
    // the comments in AVL.hpp for details about them.
    TreeNode*& _find(const K& key, TreeNode*& cur) const;
    const D& _remove(TreeNode*& node);
    
    // _remove relies on the following functions. (The organization is a
    // little different from the bst directory example.)

    // _iopRemove: targetNode is the node to remove, and iopNode is the
    // in-order predecessor to be used in a swap (which must be found first).
    // The single-argument version of _iopRemove calls the other version,
    // just as a convenience wrapper to get things started.
    const D& _iopRemove(TreeNode*& targetNode);
    const D& _iopRemove(TreeNode*& targetNode, TreeNode*& iopNode);
    TreeNode*& _swap_nodes(TreeNode*& node1, TreeNode*& node2);

    // Update the height of the specified node, based on the existing
    // heights of its children, which we assume are correct. (This function
    // is not recursive. It can do a fast update of the specified node but
    // this relies on the node's children already having correct, updated
    // heights.)
    void _updateHeight(TreeNode*& cur);

    // Ensure that the balance factor of specified node has magnitude
    // no greater than 1. This calls rotation functions as necessary to
    // rebalance the subtree rooted here. As a result, we should also
    // subsequently call this function on those nodes up the path of
    // ancestory towards the root node.
    void _ensureBalance(TreeNode*& cur);

    // These functions perform the specified balancing rotation on the
    // subtree that is rooted at the specified node.
    // Refer to the implementations of _rotateLeft and _rotateRightLeft
    // for more details. The other functions are similar.
    void _rotateLeft(TreeNode*& cur);
    void _rotateRight(TreeNode*& cur);
    void _rotateRightLeft(TreeNode*& cur);
    void _rotateLeftRight(TreeNode*& cur);

  public:

    // get_height: A wrapper for checking the height of a subtree.
    // If a node doesn't exist (nullptr), then return -1.
    // Otherwise, return the node's previously-recorded subtree height.
    // (This function was just shown as "height(...)" in some of the lecture
    //  videos. It's been renamed here to clarify that it's different from
    //  the node's height member variable.)
    int get_height(const TreeNode*& node) const {
      if (!node) {
        // A non-existent node has a height of 1
        return -1;
      }
      else {
        // We assume that an existing node already has an updated height
        return node->height;
      }
    }

    // get_balance_factor: A helper function for safely calculating the balance
    // factor of the node that is passed as the argument.
    int get_balance_factor(const TreeNode*& node) const {
      if (!node) {
        // A non-existent node has a balance factor of 0
        return 0;
      }
      else {
        // Calculate the balannce factor safely and return it.
        return get_height(node->right) - get_height(node->left);
      }
    }

    // empty: Tells whether the tree is empty or not.
    bool empty() const {
      // If the head_ pointer is null, returns true. Else, returns false.
      // (This happens implicitly because the value of head_ is cast to
      // bool and logically negated.)
      return !head_;
    }

  private:

    // printInOrder: Print the tree contents to std::cout using an in-order
    // traversal. The "_printInOrder" version is for internal use by the
    // public wrapper function "printInOrder".
    void _printInOrder(TreeNode* node) const {
      // Base case: if node is nullptr, then print a space and return.
      if (!node) {
        std::cout << " ";
        return;
      }
      else {
        // Recurse left:
        _printInOrder(node->left);
        // Print this node:
        std::cout << "[" << node->key << " : " << node->data << "]";
        // Recurse right:
        _printInOrder(node->right);
      }
    }

  public:

    void printInOrder() const {
      _printInOrder(head_);
    }

    // Prints the tree vertically without any fancy formatting in the margin.
    // For debugging purposes.
    void printVertical() const;

    // clear_tree: Remove the head item until the tree is empty.
    void clear_tree() {
      while (head_) {
        // As long as the head pointer isn't null, we can just look at
        // what the key is and call remove based on that.
        remove(head_->key);
      }
    }

    // Destructor: We just clear the tree.
    // This is public but you shouldn't call it directly. It gets called
    // when your tree is destroyed, either when you use "delete" on a tree
    // stored on the heap or when a tree stored on the stack goes out of
    // scope.
    ~AVL() {
      clear_tree();
    }

};

// Note 1:
// That this implementation of a tree is storing explicit aliases to memory
// using references. This means that the actual allocation, cleanup, and
// general responsibility for that data memory will lie outside of this class
// implementation. However, the tree nodes themselves belong to the tree and
// will need to be created or deleted by the tree.
//   There is an extra layer of complication in that plain C++ references
// cannot be re-assigned to refer to something else; they must be initialized
// to refer to one thing, and then they always refer to the same thing. You
// can edit the data they refer to, but you can't change them to refer to
// other data. However, if you are using "const references" as well, as this
// version of a tree does, then the data itself cannot be edited either. The
// references it stores are then direct alias to other memory in a read-only
// mode.
//   Please see further comments in the file: AVL.hpp

// Note 2:
// This TreeNode constructor initializes its own "data" member using
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
#include "AVL.hpp"

