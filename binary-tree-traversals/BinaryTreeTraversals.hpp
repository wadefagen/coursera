/**
 * BinaryTreeTraversals.hpp - auxiliary definitions for tree traversal example
 *
 * @author
 *   Eric Huber, Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#pragma once

#include "BinaryTreeTraversals.h"

// Note that this .hpp file is just another header file, to be included
// by the first header file. The idea is that templated class code needs
// to be defined in headers, but you still might want to neatly separate
// the code into separate files.

// We still wrote the preprocessor directives shown above, to illustrate
// a common way to handle this situation: If the users mistakenly include
// the wrong header when they use your library, it won't matter this way,
// because either header will correctly pull in the other header.

// -------

// createCompleteTree function implementation:
// The contents of vector "contents" will be arranged in the tree level by
// level, from left to right, as a complete binary tree. For example,
// passing in a vector of integers {1, 2, 3, 4} will store a copy of 1 in a
// root node, 2 will be the left child of the 1, 3 will be the right child
// of the 1, and 4 will be the left child of the 2.
template <typename T>
void BinaryTree<T>::createCompleteTree(const std::vector<T>& contents) {

  // Begin by clearing anything that we had previously put in the tree.
  destroyWholeTree();

  // If the input was empty for some reason, just leave the tree empty
  // and return early.
  if (contents.empty()) return;

  // Otherwise there's at least one item in the contents.
  // Create the root node as a pointer to a new node address on the heap.
  // Remember that the "root_" member variable already exists in scope,
  // and it's a pointer to the TreeNode type for our current templated tree
  // type. The "new" operator returns an address that we can assign to
  // a pointer variable.
  root_ = new TreeNode(contents[0]);

  // Now we want to put in the rest of the layers.

}

