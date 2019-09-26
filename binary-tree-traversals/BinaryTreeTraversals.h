/**
 * BinaryTreeTraversals.h - class outline for tree traversal example
 * 
 * @author
 *   Eric Huber, Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#pragma once

#include <iostream>
#include <vector>
#include <queue>

template <typename T>
class BinaryTree {
  public:

    // We'll start with this node type declaration that's specific to this
    // variety of BinaryTree<T>. It's okay for this to be public; it's just
    // a type.
    class TreeNode {
      public:
        // For simplicity, this version of the tree will store value copies
        // of the actual data items placed in the tree, not references.
        // (That is, note that we wrote "T data" here, not "T& data".)
        T data;
        TreeNode *left, *right;
        // This TreeNode constructor makes a copy of the data passed in when
        // it initializes its "data" member variable using the input called
        // "dataArgument". It copies the value since "data" is defined simply
        // with type "T". We only use an initialization list, so the function
        // body is written empty as "{}" to complete the definition.
        TreeNode(const T & dataArgument) : data(dataArgument), left(nullptr),
          right(nullptr) { }
    };

    // Default constructor: We'll declare define this explicitly here.
    // Make sure that the root pointer is initialized as null.
    // We can use an initialization list for that and the function body
    // may be empty here to complete the definition.
    BinaryTree() : root_(nullptr) { }

    // We'll inhibit the compiler from allowing a copy constructor or from
    // creating an implicit copy constructor for us. This is a special use
    // of syntax for this purpose, not the ordinary "delete" the deallocates
    // heap memory.
    // (If you do want a copy constructor for this data structure,
    // you need to make sure to walk the tree you are copying and do a deep
    // copy, not a shallow copy. We'll just disable this at compile time
    // to prevent ourselves from accidentally doing a shallow copy with an
    // implicitly-defined copy constructor.)
    BinaryTree(const BinaryTree& other) = delete;

    // Destructor:
    // We just use our recursive destroyWholeTree function (defined below).
    // (Note that we can call member functions within the class definition
    // like this before we even declare them. That's a peculiarity of class
    // definitions in C++, because of how the compiler preprocesses the
    // whole definition.)
    ~BinaryTree() {
      destroyWholeTree();
    }

    // This function will let us build a "complete" tree based on the vector
    // of intended contents that is passed in. We'll declare this function
    // here and define it in BinaryTree.hpp separately. Please see that file
    // for important notes on this!
    void createCompleteTree(const std::vector<T>& contents);

    // As a convenience, we'll let ourselves invoke the createCompleteTree
    // function directly from a constructor with an argument like this.
    // First it delegates the initial initialization of an empty tree to the
    // default constructor. Then, we call the member function.
    BinaryTree(const std::vector<T>& contents) : BinaryTree() {
      createCompleteTree(contents);
    }

    // This function deletes the subtree rooted at the specified node.
    // Note that this is a recursive function.
    void destroySubtree(TreeNode* subtreeRootPtr) {
      // Base case: If the specified pointer is nullptr, then just return early.
      // Using the "not" operator (!) on the pointer is another way to check this.
      if (!subtreeRootPtr) {
        return;
      }

      // General case:

      // Process the child subtrees first. These calls do nothing if the
      // child nodes don't exist, because of the base case above that handles
      // null pointers correctly.
      destroySubtree(subtreeRootPtr->left);
      destroySubtree(subtreeRootPtr->right);

      // Now that the children are already destroyed (which we assume because
      // recursion happened!), let's zero out our pointers to those children
      // just to avoid making mistakes. We should not use "delete" on these
      // pointers here, because they were already deleted during recursion!
      subtreeRootPtr->left = nullptr;
      subtreeRootPtr->right = nullptr;

      // Finally, we can delete the main subtree pointer for this call.
      // (Whoever called this function originally should set their pointer
      // to nullptr outside, just to be safe.)
      delete subtreeRootPtr;
    }

    // This is a wrapper for destroySubtree that just destroys everything
    // from the root down.
    void destroyWholeTree() {
      destroySubtree(root_);
      root_ = nullptr;
    }
  
  private:

    TreeNode *root_;

};

// Sometimes, your header files might include another header file with
// further templated definitions. The .h and .hpp are both just filename
// extensions for header files.
#include "BinaryTreeTraversals.hpp"

