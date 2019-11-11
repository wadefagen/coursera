/**
 * ValueBinaryTree.hpp - auxiliary definitions for tree traversal example
 *
 * @author
 *   Eric Huber, Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#pragma once

#include "ValueBinaryTree.h"

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
void ValueBinaryTree<T>::createCompleteTree(const std::vector<T>& contents) {

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

  // Diagnostic code:
  // std::cerr << "Added root node with data: " << root_->data << std::endl;

  // Make a queue for the next child pointers that we need to assign data
  // to, based on loading the data from the input vector into a complete
  // binary tree, layer by layer. It would be convenient to put references
  // here, something like TreeNode*&, to assign to each existing pointer,
  // but we can't normally store references in an STL container. Instead
  // we can use TreeNode**, pointers to pointers to TreeNodes. (We'll name
  // our queue with "PtrPtr" just to remember.)
  std::queue<TreeNode**> childPtrPtrQueue;

  // If you don't like using pointers-to-pointers, there is something called
  // std::reference_wrapper that can help you store references in containers.
  // We might show an example of that later on. For now, let's practice
  // thinking with pointers and addresses.

  // Push the root node's left and right child pointers onto the queue, in
  // that order. Note that each child pointer is of type TreeNode* already,
  // but we want to store a pointer to a pointer to a TreeNode. So, we need
  // to store the ADDRESS of each child pointer here, using the address-of
  // operator, "&".
  childPtrPtrQueue.push(&(root_->left));
  childPtrPtrQueue.push(&(root_->right));

  // Now we want to put in the rest of the layers by looping over the
  // remaining contents and popping the next children pointers from the
  // queue. As we create new nodes, we'll need to push more pointers onto
  // the queue as well.
  //   There are a lot of ways to write this loop. We could use standard
  // library iterators to traverse the vector safely, but for simplicity
  // we'll just use index notation. We need to skip the first item of index 0
  // because we already put it in the root, so we begin with i=1.
  //  Remember that arrays are indexed from 0, so the last item has the index
  // size()-1. For example, with 3 items in an array, they are indexed 0,1,2.
  // The vector's size() is 3, and the last index is size()-1, which is 2.
  // You can write "i <= contents.size()-1", but we'll just write it as
  // "i < contents.size()".
  for (unsigned int i = 1; i < contents.size(); i++) {

    // Get the next child pointer pointer from the queue. We know we should
    // always have some available here. Why? Because we put in two at the
    // beginning, and now, every time we remove one, we put two more back.
    // That means there is always a net increase in the number of child
    // pointers.
    //   Remember that with std::queue, the front() function lets you look
    // at the next item that will be removed, and the pop() function removes
    // that item WITHOUT returning it. So we need to use both separately.
    TreeNode** childPtrPtr = childPtrPtrQueue.front();
    childPtrPtrQueue.pop();

    // Now that we have a pointer to pointer to TreeNode, it's a little
    // confusing, so let's be careful. Dereferencing this one time gives us
    // just a pointer to TreeNode; in fact, it gives us the ACTUAL pointer
    // that's part of our tree structure, which is what we want:
    //  (childPtrPtr) has type TreeNode**
    //  (*childPtrPtr) has type TreeNode*
    // So let's use a reference to pointer type to make a direct alias to
    // this tree node pointer now. This way, we can change that actual node
    // pointer in-place to point to something new.
    TreeNode*& actualChildPtr = *childPtrPtr;

    // Now we can allocate a new TreeNode in the correct place with the
    // current item of contents.
    actualChildPtr = new TreeNode(contents[i]);

    // Diagnostic code:
    // std::cerr << "Added child node with data: " << actualChildPtr->data << std::endl;

    // With our new node created, we need to push pointers-to-pointers to the
    // left and right children onto our queue. Remember to store the addresses
    // of the child pointers, since we are making pointers-to-pointers.
    childPtrPtrQueue.push(&(actualChildPtr->left));
    childPtrPtrQueue.push(&(actualChildPtr->right));

  }

  // Now that the loop is over, we've loaded all the contents into the tree.
  // The queue still has a lot of pointers-to-pointers in it that we didn't
  // use. That's normal: those are all the child pointers remaining on the
  // leaves of the tree. When our function returns, the queue will go out
  // of scope, and the extra memory it was using will be released. We don't
  // need to write anything else here.

}

// -------

// Traversal functions:

template<typename T>
void ValueBinaryTree<T>::preOrder(TreeNode* cur) {
  // Only do anything if the pointer is not nullptr.
  if (cur) {
    // We'll act upon this node first, by "shouting it out" now. This is
    // our way of officially visiting this node at this time, as part of the
    // intended "pre-order" traversal.
    shout(cur);
    // Then, recurse on the left.
    preOrder(cur->left);
    // Lastly, recurse on the right.
    preOrder(cur->right);
  }
}

template<typename T>
void ValueBinaryTree<T>::inOrder(TreeNode* cur) {
  // Only do anything if the pointer is not nullptr.
  if (cur) {
    // First, recurse on the left.
    inOrder(cur->left);
    // Now act upon this node, because we doing an "in-order" traversal.
    shout(cur);
    // Lastly, recurse on the right.
    inOrder(cur->right);
  }
}


template<typename T>
void ValueBinaryTree<T>::postOrder(TreeNode* cur) {
  // Only do anything if the pointer is not nullptr.
  if (cur) {
    // First, recurse on the left.
    postOrder(cur->left);
    // Next, recurse on the right.
    postOrder(cur->right);
    // Now act upon this node last, because we doing a "post-order" traversal.
    shout(cur);
  }
}

// What about a level-order traversal? You'll try doing that in one of the
// programming projects.

// Hint 1: But actually, if you study the source code for ValueBinaryTree in
// the files here, you'll see that createCompleteTree basically performs a
// level-order traversal already.

// Hint 2: The essential mechanism that allows us to do the level-order
// traversal in createCompleteTree is the queue that helps us order our
// visits, not the use of the pointer-to-pointer type that we use for
// assembling the tree.

