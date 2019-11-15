/**
 * AVL class outline
 *
 * @author Wade Fagen-Ulmschneider <waf@illinois.edu>
 * @author Eric Huber
 */

// This example is based on original work by Prof. Wade Fagen-Ulmschneider
// as shown in lecture. We implement some remaining functions that had not
// been shown in lecture, as well as some additional helper functions for
// clarity of code reading and for debugging. (The code checks itself for
// correctness when you compile and run the included main program.) Since
// the code is not identical to the fragments shown in lecture, the line
// numbers from lecture will not match.

// Notes from TA Eric:
// If you are studying AVL from the lectures, focus first on the theory of
// how the AVL structure is meant to work. The very specific details of the
// implementation choices here are not necessarily crucial to understanding
// AVL in general. There are other, possibly simpler, ways to implement it.

// The recursive nature of this example is a bit complicated so many comments
// have been added to explain how it works. (Some of the comments may seem a
// bit redundant. I can't assume anyone will read the whole thing from start
// to finish.) There are many helper functions that break out the logic into
// small pieces. The easiest way to understand the correctness of the
// recursion is to focus on the logical properties of the current state of
// the program (those conditions which are required or guaranteed by each of
// the member functions) before and after each call is made. For example,
// assume that calling _ensureBalance on a node will ensure that the subtree
// there becomes balanced, which may imply that some rotations are performed.

// Before studying this example, please first study these other example
// directories:
//   binary-tree
//   binary-tree-traversals
//   bst
// These contain notes that are useful prerequisite knowledge for better
// understanding how this AVL example is implemented. In particular, this
// example has underlying similarities to the code in the bst directory, and
// many of the specific code comments in that project also apply to this
// example.

// The BST example (in /bst) contains some additional commentary about
// how both that example and this AVL example are implemented to store
// references. That drastically affects the overall design. In contrast,
// the /binary-tree-traversals example stores value copies in the nodes
// instead of references, so its fundamental approach to memory usage is
// very different.

// As mentioned above, this AVL example has been revised and expanded
// from the fragments shown in lecture. Here are some substantial changes:
// - The node swapping function has been changed. (The lectures show the
//   calls being made to this function but not how it is implemented. As the
//   implementation has been changed, the function calls also look slightly
//   different, and now the function returns something. The swap algorithm
//   is also discussed in the commentary on the bst example.)
// - The _iopRemove method was changed to reflect the changes to the node
//   swapping function. The _iopRemove function has an extra parameter to
//   track whether it's the first step in recursion or not. In addition,
//   there is a separate version of the _iopRemove provided as a wrapper
//   function to simplify the beginning of the recursion process.
// - The lectures implied the existence of a helper function called "height".
//   For clarity, this has been renamed to "_get_height".
// - The insert and remove methods have additional helper functions that are
//   similar to _find, but help to simplify the rebalancing process during
//   recursion.
// - The _ensureBalance function accounts for an additional possibility where
//   there is a balance of 0 in the direction of an initial imbalance. This
//   can only happen after a removal, not after an insertion.
// - Some other helper functions have been added to help with testing and
//   debugging the implementation. Some of these cause the implementation
//   to run much slower than the theoretical speed of an AVL tree, but they
//   could be deactivated if desired.

#pragma once

// We include <stdexcept> so we can throw std::runtime_error in some cases.
#include <stdexcept>
// We include <utility> for the std::swap function
#include <utility>
// We'll add some tree printing functions to help us inspect the results.
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

    // "contains" is like "find", but it doesn't return a reference to
    // the data item. It just returns a bool indicating whether the item
    // exists or not. If the item is not found, we don't need to throw
    // an exception.
    bool contains(const K& key);

  private:
    class TreeNode {
      public:
        // *See note 1 at the bottom of this file for discussion about how
        // references are being used here.
        const K& key;
        const D& data;
        // Note that you can declare multiple pointers on the same line as
        // shorthand, like this:
        //   TreeNode *left, *right;
        // But since this requires you to write the "*" with each variable
        // name, it can be a little confusing, or prone to making a mistake.
        // Instead, you can declare the pointers on separate lines like this:
        TreeNode* left;
        TreeNode* right;
        int height;
        // **See note 2 at the bottom of this file for discussion about how
        // this initialization list is styled.
        TreeNode(const K& key, const D& data)
          : key(key), data(data), left(nullptr), right(nullptr), height(0) { }
    };

    TreeNode* head_;

    // These internal helper functions are private because they are only
    // meant to be used by other member functions of our class. Please see
    // the comments in AVL.hpp for details about them.

    // The naming convention may be a little bit confusing.
    // Here is an overview of how these functions call each other
    // sequentially when the public functions are used:
    // find -> _find
    // insert -> _find_and_insert (-> possibly, rotations)
    // remove -> _find_and_remove -> _remove (-> _iopRemove, swapping, rotations)

    // Find the node with this key and return its data.
    TreeNode*& _find(const K& key, TreeNode*& cur) const;

    // Actually remove the node that this pointer points to.
    // (This may need to invoke _iopRemove in some cases.)
    const D& _remove(TreeNode*& node);

    // This is called by "insert" to recursively find the insertion point,
    // insert, and rebalance as it returns.
    void _find_and_insert(const K& key, const D& data, TreeNode*& cur);
    // This is called by "remove" to recursively remove the correct node
    // and rebalance as it returns.
    const D& _find_and_remove(const K& key, TreeNode*& cur);

    // _remove relies on the following functions. (The organization is a
    // little different from the bst directory example.)

    // _iopRemove: targetNode is the node to remove, we'll want to find the
    // in-order predecessor (IOP) and swap the target with it for removal.
    // To recursively find the IOP, we traverse over the ancestors of the
    // IOP until we find it. The single-argument version of _iopRemove calls
    // the other version of _iopRemove on the first node to the left, which
    // is the earliest ancestor; this makes things more convenient and helps
    // us avoid making a mistake in the logic.
    const D& _iopRemove(TreeNode*& targetNode);
    // The iopAncestor argument tracks the nodes being traversed on the way
    // down as we search for the IOP that can be swapped with the target.
    // When the single-parameter version of _iopRemove calls this other
    // version, it needs to pass "true" for the isInitialCall argument.
    // We check that in the implementation to catch a certain edge case.
    const D& _iopRemove(TreeNode*& targetNode, TreeNode*& iopAncestor, bool isInitialCall);

    // _swap_nodes: This swaps the node positions (rewiring pointers as
    // necessary) and also swaps the node heights. The intended usage is
    // that node1 is higher (such as some internal node to remove) and node2
    // is one of its descendants (especially the IOP). After calling this,
    // the original node pointers that were passed to the function shouldn't
    // be directly used again, because it's hard to tell what they point to
    // afterward. Instead, the function returns a pointer, by reference,
    // that is the updated node1 pointer.
    TreeNode*& _swap_nodes(TreeNode*& node1, TreeNode*& node2);

    // Update the height of the specified node, based on the existing
    // heights of its children, which we assume are correct. (This function
    // is not recursive. It can do a fast update of the specified node but
    // this relies on the node's children already having correct, updated
    // heights.)
    void _updateHeight(TreeNode*& cur);

    // Ensure that the balance factor of specified node has magnitude
    // no greater than 1. This calls rotation functions as necessary to
    // rebalance the subtree rooted here. It also records the updated height
    // in the node. These changes need to cascade upward, so after we call
    // this function, we need to make sure that it also gets called on those
    // nodes on the path of ancestry up towards the root node.
    void _ensureBalance(TreeNode*& cur);

    // These functions perform the specified balancing rotation on the
    // subtree that is rooted at the specified node.
    // Refer to the implementations of _rotateLeft and _rotateRightLeft
    // for more details. The other functions are similar.
    void _rotateLeft(TreeNode*& cur);
    void _rotateRight(TreeNode*& cur);
    void _rotateRightLeft(TreeNode*& cur);
    void _rotateLeftRight(TreeNode*& cur);

    // _get_height: A wrapper for checking the height of a subtree.
    // If a node doesn't exist (nullptr), then return -1.
    // Otherwise, return the node's previously-recorded subtree height.
    // (This function was just shown as "height(...)" in some of the lecture
    //  videos. It's been renamed here to clarify that it's different from
    //  the node's height member variable.)
    int _get_height(TreeNode*& node) const {
      if (!node) {
        // A non-existent node has a height of 1
        return -1;
      }
      else {
        // We assume that an existing node already has an updated height
        return node->height;
      }
    }

    // _get_balance_factor: A helper function for safely calculating the balance
    // factor of the node that is passed as the argument.
    int _get_balance_factor(TreeNode*& node) const {
      if (!node) {
        // A non-existent node has a balance factor of 0
        return 0;
      }
      else {
        // Calculate the balance factor safely and return it.
        return _get_height(node->right) - _get_height(node->left);
      }
    }

  public:

    // empty: Tells whether the tree is empty or not.
    bool empty() const {
      // If the head_ pointer is null, returns true. Else, returns false.
      // (This happens implicitly because the value of head_ is cast to
      // bool and logically negated.)
      return !head_;
    }

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

  public:
    // Prints the tree vertically without any fancy formatting in the margin.
    // For debugging purposes.
    void printVertical() const;

  public:
    // printInOrder: Print the tree contents to std::cout using an in-order
    // traversal. The "_printInOrder" version is for internal use by the
    // public wrapper function "printInOrder".
    void printInOrder() const;
  private:
    void _printInOrder(TreeNode* node) const;

  public:
    // More debugging functions to help check the AVL tree properties.
    // These are not meant to be fast and they are optional components.
    // I haven't fully commented how they work but the implementations
    // are in AVL-extra.hpp.
    bool runDebuggingChecks();
  private:
    bool _debugHeightCheck(TreeNode* cur);
    bool _debugBalanceCheck(TreeNode* cur);
    bool _debugOrderCheck(TreeNode* cur);

    // This constant controls whether debugging checks will be run after
    // insertions and removals. Note that this makes the data structure run
    // below its intended theoretical speed.
    //   Also, here's an obscure syntax note:
    // It's possible to give this an initial value here because it's a
    // compile-time constant (constexpr) and of bool type, and it's static
    // (shared for the whole class), and it's a templated class. That is
    // pretty specific... You can't do this for all member variables of all
    // types. In newer versions of C++, C++17 and later, there are more
    // flexible and consistent ways to define constants like this for the
    // whole class. This version of the example has been written with C++14
    // compatibility in mind.
    static constexpr bool ENABLE_DEBUGGING_CHECKS = true;

};

// (Very obscure syntax note)
// C++14 compatibility: This external definition of the static constexpr is
// sometimes needed in C++14, but it is deprecated in C++17 and later.
// In any case, the actual setting is initialized in the class definition
// itself where this member is first mentioned.
template <typename K, typename D>
constexpr bool AVL<K, D>::ENABLE_DEBUGGING_CHECKS;

// (Note 1) About how each TreeNode stores references:
//   That this implementation of a tree is storing explicit aliases to memory
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

// (Note 2) About referring to variable names in an initialization list:
//   This TreeNode constructor initializes its own "data" member using
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

