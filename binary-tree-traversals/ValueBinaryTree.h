/**
 * ValueBinaryTree.h - class outline for tree traversal example
 * 
 * @author
 *   Eric Huber, Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#pragma once

#include <iostream>
#include <vector>
#include <queue>

// This version of a binary tree has been named "ValueBinaryTree" to point
// out that it stores value copies of items, rather than references,
// and so to avoid confusion with some of the other examples using a class
// named "BinaryTree" that stores references.

// We'll use this version to demonstrate tree traversals, but this specific
// implementation of this class isn't shown in any particular lecture.
// However, we will follow the outline for the traversal functions as they
// are shown in the lecture on tree traversals.

template <typename T>
class ValueBinaryTree {
  public:

    // We'll start with this node type declaration that's specific to this
    // variety of ValueBinaryTree<T>. It's okay for this to be public; it's just
    // a type.
    class TreeNode {
      public:
        // VERY IMPORTANT:
        // For simplicity, this version of the tree will store value copies
        // of the actual data items inserted in the tree, NOT references.
        // (That is, note that we wrote "T data" here, not "T& data".)
        T data;
        // Note that you can declare multiple pointers on the same line as
        // shorthand, like this:
        //   TreeNode *left, *right;
        // But since this requires you to write the "*" with each variable
        // name, it can be a little confusing, or prone to making a mistake.
        // Instead, you can declare the pointers on separate lines like this:
        TreeNode* left;
        TreeNode* right;
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
    ValueBinaryTree() : root_(nullptr) { }

    // We'll inhibit the compiler from allowing a copy constructor or from
    // creating an implicit copy constructor for us. This is a special use
    // of syntax for this purpose, not the ordinary "delete" the deallocates
    // heap memory.
    // (If you do want a copy constructor for this data structure,
    // you need to make sure to walk the tree you are copying and do a deep
    // copy, not a shallow copy. We'll just disable this at compile time
    // to prevent ourselves from accidentally doing a shallow copy with an
    // implicitly-defined copy constructor.)
    ValueBinaryTree(const ValueBinaryTree& other) = delete;

    // Destructor:
    // We just use our recursive destroyWholeTree function (defined below).
    // (Note that we can call member functions within the class definition
    // like this before we even declare them. That's a peculiarity of class
    // definitions in C++, because of how the compiler preprocesses the
    // whole definition.)
    ~ValueBinaryTree() {
      destroyWholeTree();
    }

    // This function will let us build a "complete" tree based on the vector
    // of intended contents that is passed in. We'll declare this function as
    // a member of the class here by writing its function prototype, and then
    // we'll define the full implementation in ValueBinaryTree.hpp separately.
    void createCompleteTree(const std::vector<T>& contents);

    // As a convenience, we'll let ourselves invoke the createCompleteTree
    // function directly from a constructor with an argument like this.
    // First it delegates the initial initialization of an empty tree to the
    // default constructor. Then, we call the member function.
    ValueBinaryTree(const std::vector<T>& contents) : ValueBinaryTree() {
      createCompleteTree(contents);
    }

    // This function deletes the subtree rooted at the specified node.
    // Note that this is a recursive function.
    void destroySubtree(TreeNode* subtreeRootPtr) {
      // Base case: If the specified pointer is nullptr, then just return
      // early. (Using the "not" operator (!) on the pointer is another way
      // to check when it is null.)
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

    // This function will "shout out" the value at the specified node, as the
    // professor describes it in the lecture on tree traversals. The idea is
    // to perform actual work on a given node by doing some action when it is
    // officially "visited" during the tree traversal (that is, when we are
    // not just traversing past it temporarily to access other nodes). For
    // the action in this case, we'll just print the value to the standard
    // output stream, hence we call it "shout". We assume that type T, which
    // we store copies of in each node here, must be some type that supports
    // output with "std::cout <<" in this way through an operator overload.
    // Many basic types like int or std::string do support that.
    void shout(TreeNode* cur) {
      // Only try to dereference the node pointer if it is not nullptr.
      // We can check that just by testing if the pointer's value is nonzero,
      // which happens if you let it be automatically converted to a Boolean
      // value in a conditional test:
      if (cur) {
        // We'll dereference the pointer to get the data, and output that.
        std::cout << cur->data;
      }
      // We'll always output a blank space afterwards, no matter what.
      // This will help us read the output.
      std::cout << " ";
    }

    // Traversal functions:
    // More information about these will be given in ValueBinaryTree.hpp
    // where they are implemented. Here we simply declare that they exist
    // as members of the class by writing their function prototypes.
    void preOrder(TreeNode* cur);
    void inOrder(TreeNode* cur);
    void postOrder(TreeNode* cur);

    // This function lets the user directly obtain the root pointer for
    // manual operations on the tree contents. This is generally not safe
    // to expose to your users without at least signifying that the function
    // is unsafe somehow. It would be better to write a set of functions for
    // safely iterating over and inserting nodes. For the sake of this example
    // program, we can quickly specify a complete tree to build using some of
    // the other functions defined in this class, and then to make additional
    // tweaks to an example tree, we might want to directly access the node
    // pointers this way.
    TreeNode* unsafe_getRootPtr() {
      return root_;
    }
  
  private:

    TreeNode *root_;

};

// Sometimes, your header files might include another header file with
// further templated definitions. The .h and .hpp are both just filename
// extensions for header files.
#include "ValueBinaryTree.hpp"

