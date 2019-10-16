/**
 * Dictionary BST auxiliary definitions.
 * 
 * (Some versions of the lecture videos called this file "Dictionary.cpp",
 *  but we are renaming it to "Dictionary.hpp" as it includes templated
 *  implementations and would typically be included as a header.
 *  The line numbers in this file do not exactly match what is shown in the
 *  lecture slides. Please read the comments for further explanation.)
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>, Eric Huber
 */

#pragma once

#include "Dictionary.h"

// Note that this .hpp file is just another header file, to be included
// by the first header file. The idea is that templated class code needs
// to be defined in headers, but you still might want to neatly separate
// the code into separate files.

// We still wrote the preprocessor directives shown above, to illustrate
// a common way to handle this situation: If the users mistakenly include
// the wrong header when they use your library, it won't matter this way,
// because either header will correctly pull in the other header.

// -------

// Additional implementation notes from TA Eric:
//   Some of the code shown below was not shown directly in lecture, and it
// differs slightly in some parts, as noted. Rationale for the changes:
// I split the action of the original "_iop" function call into separate
// functions, "_iop_of" and "_rightmost_of", as the original call required
// part of the method to be performed in the argument list; in main.cpp, I
// added a separate storage vector to avoid retaining references to temporary
// objects, which some compilers may not support; and the node-swapping
// function was redesigned to resolve some issues, which also required adding
// a return value. These are subtle issues that aren't discussed directly in
// lecture and they don't affect the underlying lessons.
//   This is quite a tricky implementation since nearly all of the member
// functions pass and return references to the actual pointers that make
// up the tree structure. Therefore it edits the tree in-place with almost
// every action, rather than passing copies of anything. Remember that we
// can return references to data that is persisting in memory storage else-
// where; in this case, the actual data that the tree organizes is being
// stored somehow outside of the tree itself (see an example in main.cpp),
// and the tree just records references to that external storage. We can also
// return references to the tree's own data members that persist between
// function calls, such as the actual pointers between nodes that it stores.
// However, do remember that you must never return a reference to a temporary
// stack variable that you create in some function; that memory would become
// immediately invalid after the function call.
//   There are pros and cons to implementing a tree this way. We can edit a
// parent's connection to its child without storing pointers to parents or
// trying to traverse upwards; instead, we assume that when pointers are
// passed by reference, we are editing the same variable that the parent is
// holding.
//   The big challenge with using plain references is that they keep
// referring to the same thing after being first initialized. If you need to
// re-bind the reference to something else, it's easier to use pointers; you
// can make pointers to pointers for layers of indirection, and explicitly
// control how those addresses are stored and dereferenced. There is also an
// advanced mechanism called std::reference_wrapper that you can use to
// simulate storing references that may be re-bound, as many STL containers
// would require.
//   For the node data itself, instead of referencing external items, you
// could instead just store value-based copies, which is very easy to write
// but less memory-efficient. We have an example in the directory
// "binary-tree-traversals". But there are many other advanced ways to do
// that in C++ as well. There are some additional notes about the options for
// the underlying data storage in main.cpp in the current directory.

// ------

template <typename K, typename D>
const D& Dictionary<K, D>::find(const K& key) {
  // Find the key in the tree starting at the head.
  // If found, we receive the tree's actual stored pointer to that node
  //   through return-by-reference.
  // If not found, then the node returned has a value of nullptr.
  TreeNode*& node = _find(key, head_);
  if (node == nullptr) { throw std::runtime_error("error: key not found"); }
  // We found the node, so return the actual data there, by reference.
  return node->data;
}

// Note about the use of "typename" in the below definition:
// This is required so that although we're writing at global scope here, we
// can refer to the TreeNode type definition that is part of Dictionary.
// Since we're writing this function definition outside of the primary class
// definition, the compiler needs help to understand that what follows is
// expected to be the name of a type defined within a specific templated
// class.

// Newer versions of the C++ standard may be smarter about auto-detecting
// this. Until then, it's hard to summarize when you need to write
// "typename" at global scope and when you don't, but in general, you might
// need to write it before a templated type, especially a type belonging to
// another class namespace. If you get a compiler error that looks like this:
//  "missing 'typename' prior to dependent type name"
//  "need 'typename' before ... because ... is a dependent scope"
//   (etc.)
// then you probably need to put "typename" before the type.

// The fully-qualified return type of the below function is:
// Dictionary<K, D>::TreeNode*&
// That is a pointer to a Dictionary<K, D>::TreeNode, returned by reference.

template <typename K, typename D>
typename Dictionary<K, D>::TreeNode*& Dictionary<K, D>::_find(
  const K& key, TreeNode*& cur) const {

  // (Please also see the implementation of _iop_of below, which discusses
  //  some more nuances about returning references to pointers.)

  // [Base case 1: When the key is not found]
  // cur will be nullptr if the tree is empty, or if we descend below the
  // lowest level without finding the key. Then we return nullptr and the
  // outer "find" function (which calls "_find") will report that as an
  // error. Or, if we were calling insert, then the pointer returned is the
  // position where the item should be placed.
  //   Note: The "cur" we return in this case is equal to nullptr, but
  // it's important to write "return cur;" and not "return nullptr;" since
  // this function returns by reference. We specifically want to return the
  // pointer at this position we found. This is true whether we want to
  // replace it, as when we're doing an insertion, or if this is a failed
  // "find" operation that should report an error. We should not return a
  // reference to the "nullptr" literal, and we should avoid making
  // references to temporary constants like numerical literals in any case.
  if (cur == nullptr) { return cur; }
  // [Base case 2: When the key is found]
  // If we find a key that matches by value, then return the current TreeNode*
  else if (key == cur->key) { return cur; }
  // [When we need to search left]
  // If the key we're looking for is smaller than the current node's key,
  // then we should look to the left next.
  else if (key < cur->key) { return _find(key, cur->left); }
  // [When we need to search right]
  // Otherwise, implicitly, the key we're looking for is larger than the
  // current node's key. (We know this because it's not equal and not less.)
  // So we should search to the right next.
  else { return _find(key, cur->right); }

}

/**
* insert()
* Inserts `key` and associated `data` into the Dictionary.
*/
template <typename K, typename D>
void Dictionary<K, D>::insert(const K& key, const D& data) {
  // Find the place where the item should go.
  TreeNode *& node = _find(key, head_);
  // For the sake of this example, let's disallow duplicates. If the node
  // found isn't a nullptr, then the key already exists, so report an error.
  // (We could also do something nicer than this, like remove the old key and
  // then insert the new item to replace it.)
  if (node) { throw std::runtime_error("error: insert() used on an existing key"); }
  node = new TreeNode(key, data);
}

/**
* remove()
* Removes `key` from the Dictionary. Returns the associated data.
*/
template <typename K, typename D>
const D& Dictionary<K, D>::remove(const K& key) {
  // First, find the actual pointer to the node containing this key.
  // If not found, then the pointer returned will be equal to nullptr.
  TreeNode*& node = _find(key, head_);
  if (!node) { throw std::runtime_error("error: remove() used on non-existent key"); }
  return _remove(node);
}

// _remove will remove the node pointed to by the argument. Note that this
// will alter the pointer you pass in-place, so you should not reuse the
// pointer variable after calling this function on it. You can't be sure what
// it points to anymore after the function call.
template <typename K, typename D>
const D& Dictionary<K, D>::_remove(TreeNode*& node) {

  // If the node we are trying to remove is a nullptr, then it's an error,
  // as even if we'd like to "do nothing" here as a base case, we must return
  // a const reference to some data removed, and there is none. In practice
  // you would want to add more features to your class for handling these
  // situations efficiently in a way that makes sense for your users.
  if (!node) { throw std::runtime_error("error: _remove() used on non-existent key"); }

  // When you are studying the cases below, remember: Right now, "node" is
  // the actual pointer that this node's parent holds, which points to this
  // node. When we change "node", we are remapping the parent's connection
  // to what is below it.

  // Zero child remove:
  if (node->left == nullptr && node->right == nullptr) {
    // Peek at the data referred to by the node so we can return a reference
    // to the data later, after the tree node itself is already gone.
    const D& data = node->data;
    // The node is a leaf, so it has no descendants to worry about.
    // We can just delete it. (The slides originally showed "delete(node)".
    // Note that the syntax for "delete" is like an operator, not a function,
    // so it's not necessary to put the () when you use it.)
    delete node;
    // It's very important to set "node" to nullptr here. The parent is still
    // holding this same pointer, so we must mark that the child is gone.
    node = nullptr;
    return data;
  }
  // One-child (left) remove
  else if (node->left != nullptr && node->right == nullptr) {
    // Similar to the previous case, except that we need to remap the "node"
    // pointer to point to the node's child, so that the parent of the node
    // being deleted will retain its connection to the rest of the tree
    // below this point.
    const D& data = node->data;
    TreeNode* temp = node;
    node = node->left;
    delete temp;
    return data;
  }
  // One-child (right) remove
  else if (node->left == nullptr && node->right != nullptr) {
    // This case is symmetric to the previous case.
    const D& data = node->data;
    TreeNode* temp = node;
    node = node->right;
    delete temp;
    return data;
  }
  // Two-child remove
  else {
    // When the node being deleted has two children, we have to be very
    // careful. The lecture discusses this case in detail. (The versions
    // of the helper functions being used here are slightly different
    // compared to lecture, as noted in other comments here.)

    // Find the IOP (in-order predecessor) of the current node.
    TreeNode*& iop = _iop_of(node);

    // Old version:
    // TreeNode*& iop = _iop( node->left );
    // The lecture originally showed the call in this way, but the code has
    // been revised so that the first step to the left happens inside the
    // "_iop_of" function.

    // Since this is the two-child remove case, we know that some in-order
    // predecessor does exist, so the _iop_of lookup should not have failed.
    if (!iop) {
      throw std::runtime_error("error in two-child remove: IOP not found");
    }

    // Swap the node to remove and the IOP.
    // (This function changes the pointers that are passed in-place so that
    //  it can alter the tree structure itself, but afterwards you don't
    //  know what the pointers point to exactly, depending on what happened.
    //  Therefore you shouldn't reuse these pointers in the current function
    //  after calling this. To make things easier, _swap_nodes will return
    //  a pointer, by reference, that is the updated pointer in the tree
    //  that is now pointing to the same node we gave as the first argument,
    //  so we can do more work on that node afterwards.)
    TreeNode*& moved_node = _swap_nodes(node, iop);

    // Note that we do not use the original "node" and "iop" pointers by
    // name after this point. Assume they are invalid now.

    // Recurse to remove the original targeted node at its updated position.
    return _remove(moved_node);
  }
}

// -------

// The implementations for the following functions were not shown in the
// lecture directly. The _iop function has been revised a little bit as
// "_iop_of", which finds the IOP of the argument you pass; it traverses one
// step to the left before going to the right. This obtains the IOP of the
// "cur" node when you call "_iop_of(cur)" explicitly, instead of requiring
// a call to "_iop(cur->left)" as originally shown.

// _iop_of: You pass in a pointer to a node, and it returns the pointer to
// the in-order predecessor node, by reference. If the IOP does not exist,
// it returns a reference to a node pointer that has value nullptr.
template <typename K, typename D>
typename Dictionary<K, D>::TreeNode*& Dictionary<K, D>::_iop_of(
  TreeNode*& cur) const {

  // We want to find the in-order predecessor of "cur",
  // which is the right-most child of the left subtree of cur.

  if (!cur) {
    // If cur is nullptr, this is an error case.
    // Just return cur by reference. (Since cur has the value of nullptr,
    // we can check for that result.)
    return cur;
  }
  
  if (!(cur->left)) {
    // If cur has no left child, this is an error case.
    // Just return cur->left by reference. (Since cur->left has the value of
    // nullptr, we can check for that result.)
    return cur->left;
  }

  // --- Wait: Let's review our strategy for what to do next. ---

  // At this point, we're sure that cur is non-null and it has a left child.
  // To search for the right-most child of the left subtree, we'll begin
  // with the left child of cur, and then keep going right. However, here
  // our plans are slightly complicated since we're directly returning a
  // reference to a pointer in the tree. Let's briefly explain why. This
  // will also shed more light on how _find works.

  // We could use a simple while loop to go right repeatedly, but since we
  // ultimately need to return a reference to an actual pointer in our tree,
  // this complicates our use of a temporary pointer for looping. Remember
  // that once you initialize a reference to something, you can't make it
  // refer to something else instead. (You can use a non-const reference to
  // edit the original data being referenced, but the reference variable
  // itself will refer to the same thing for as long as the reference
  // variable exists.)

  // Here are some possible workarounds:
  // 1. Use pointers to pointers instead of references to pointers.
  //    (See the binary-tree-traversals folder for another example of this,
  //     within the ValueBinaryTree class.)
  // 2. Use std::reference_wrapper<> from the <functional> include, which
  //    lets you simulate a reference type, but can be re-assigned to refer
  //    to something else. This can also be a little confusing to use, as
  //    you need to typecast the reference_wrapper to plain reference if you
  //    want to access the actual value that is referred to. (If you choose
  //    this solution, it would make sense to rewrite this entire class to
  //    store reference_wrapper instead of plain references.)
  // 3. Use recursion with a function that takes a reference as an argument
  //    and returns a reference. This is how _find works. Since passing
  //    arguments counts as initializing the argument variable, you can
  //    simulate updating a reference with each recursive call.

  // We'll go with option #3 again, recursion, so you can get more practice
  // with that. See the binary-tree-traversals folder to learn about the
  // pointers-to-pointers concept.

  // ---------------------------------------------------------------

  // Find the rightmost child pointer under the left subtree,
  // and return it by reference.
  return _rightmost_of(cur->left);
}

// _rightmost_of:
// Find the right-most child of cur using recursion, and return that
// node pointer, by reference.
// If you call this function on a nullptr to begin with, it returns the same
// pointer by reference.
template <typename K, typename D>
typename Dictionary<K, D>::TreeNode*& Dictionary<K, D>::_rightmost_of(
  TreeNode*& cur) const {

  // Base case 1: If cur is null, then just return it by reference.
  // (Since cur has the value of nullptr, we can check for that result.)
  if (!cur) return cur;

  // Base case 2: So far, we know cur is not null. Now, if cur does not have
  // a right child, then cur is the rightmost, so return cur by reference.
  if (!(cur->right)) return cur;

  // General case: The cur pointer is not null, and it does have a right
  // child, so we should recurse to the right and return whatever we get.
  return _rightmost_of(cur->right);
}

// _swap_nodes:
// This will swap the logical positions of the nodes by changing the pointers
// in the tree. You need to be careful, because this function will change the
// two pointers in-place and invalidate them for use after the call, since
// they may not point to what you expect anymore. Instead, the function will
// will return, by reference, a pointer to the tree node that was previously
// represented by the first argument. If you need to keep track of the new
// positions of BOTH nodes after the call, for some purpose, then you could
// extend this to return two new references.
template <typename K, typename D>
typename Dictionary<K, D>::TreeNode*& Dictionary<K, D>::_swap_nodes(
  TreeNode*& node1, TreeNode*& node2) {

  // More information on the problem we need to solve here:

  // We need to swap the logical positions of these two nodes in the tree,
  // but this can be hard if one of the nodes is a child of the other. Then
  // one of the nodes holds the other actual pointer we're changing, so the
  // rewiring is a little complicated, and since we passed in these pointers
  // by reference to edit them in-place in the tree, we also end up with the
  // original argument variables becoming dangerous to reuse any further
  // outside of this function, where we made the call: These pointers no
  // longer refer to the nodes we originally thought. As a result, we need
  // to return by reference a pointer to where node1 can still be found.
  // (Again, if you want to track the new locations of both nodes, you would
  //  need to extend the return type of this function somehow, perhaps by
  //  returning a std::pair of references to pointers.)

  // These are value copies of the pointer arguments, so they hold copies
  // of the actual addresses that we are pointing to. We need to record
  // these, because when we start changing the pointers below, we'll lose
  // track of these original addresses.
  TreeNode* orig_node1 = node1;
  TreeNode* orig_node2 = node2;

  // The first case below has been fully commented, and the following cases
  // are similar and symmetric, so comments have been omitted there.

  if (node1->left == node2) {
    // When node1 has node2 as its left child...

    // The right child pointers are easy to handle.
    // We can directly swap them.
    std::swap(node1->right, node2->right);

    // Now we have to deal with the left child pointers, and it's
    // complicated. We need to be very careful about the order of changes.
    // The easiest way to see how this works is draw a diagram of the
    // original nodes and pointers, and see how the pointers have to be
    // redirected.

    // Make "node1" take its leftmost grandchild as its left child.
    // The next line also affects the actual "node2" pointer, implicitly,
    // so we won't try to use the "node2" pointer after this; it will no
    // longer point to the original "node2" node that we would expect from
    // the naming.
    node1->left = orig_node2->left;
    // Now the original node2 needs to take the original node1 as its left
    // child.
    orig_node2->left = node1;
    // Now the actual node1 pointer should point to the object that was
    // the original node2.
    node1 = orig_node2;

    // The node position swap is done, but the node1 and node2 arguments
    // themselves haven't been swapped! We can't do that in this case.

    // This is the actual pointer in the tree that is now pointing to what
    // was our original node1 object, so return it by reference.
    return node1->left;
  }
  else if (node1->right == node2) {
    std::swap(node1->left, node2->left);
    node1->right = orig_node2->right;
    orig_node2->right = node1;
    node1 = orig_node2;
    return node1->right;
  }
  else if (node2->left == node1) {
    std::swap(node2->right, node1->right);
    node2->left = orig_node1->left;
    orig_node1->left = node2;
    node2 = orig_node1;
    return node2->left;
  }
  else if (node2->right == node1) {
    std::swap(node2->left, node1->left);
    node2->right = orig_node1->right;
    orig_node1->right = node2;
    node2 = orig_node1;
    return node2->right;
  }
  else {
    // If the two nodes aren't adjacent in the tree, it's a lot easier.
    // We can swap their child pointers and swap their main pointers,
    // and it just works. (Again, the easiest way to see this is true is
    // to draw a picture of the pointer connections between the nodes.)

    std::swap(node1->left, node2->left);
    std::swap(node1->right, node2->right);
    std::swap(node1, node2);

    // This is the actual pointer in the tree that is now pointing to what
    // was our original node1 object, so return it by reference.
    return node2;
  }

  // For future reference, here are some debugging output statements that
  // might be helpful if trouble arises:

  // std::cerr << "\nAfter swap: [" << node1->key << " , " << node1->data
  //   << " ] [ " << node2->key << " , " << node2->data << " ] " << std::endl;
  // std::cerr << "\nn1: " << node1 << " n2: " << node2
  //   << "\n n1l: " << node1->left << " n1r: " << node1->right
  //   << "\n n2l: " << node2->left << " n2r: " << node2->right << std::endl;

}


