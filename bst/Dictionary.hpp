/**
 * Dictionary BST auxiliary definitions.
 * 
 * (Some versions of the lecture videos called this file "Dictionary.cpp",
 *  but we are renaming it to "Dictionary.hpp" as it includes templated
 *  implementations and would typically be included as a header.
 *  The line numbers in this file do not exactly match what is shown in the
 *  lecture slides.)
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

template <typename K, typename D>
const D& Dictionary<K, D>::find(const K& key) const {
  TreeNode*& node = _find(key, head_);
  if (node == nullptr) { throw std::runtime_error("key not found"); }
  return node->data;
}

template <typename K, typename D>
typename Dictionary<K, D>::TreeNode*& Dictionary<K, D>::_find(
  const K& key, TreeNode*& cur) const {

  // (Please also see the implementation of _iop below, which discusses
  //  some more nuances about returning references to pointers.)

  // Case 1:
  // cur will be nullptr if the tree is empty, or if we descend below the
  // lowest level without finding the key. Then we return nullptr and the
  // outer "find" function (which calls "_find") will report that as an
  // error. (Note: The "cur" we return in this case is equal to nullptr, but
  // it's important to write "return cur;" and not "return nullptr;" since
  // this function returns by reference. Returning the reference of a
  // temporary literal will often fail to compile. We can return cur by
  // reference since it's a specific pointer stored in memory in our tree
  // already, and this accomplishes what we want, since it has the value of
  // nullptr anyway.
  if      (cur == nullptr)  { return cur; }
  // Case 2:
  // If we find a key that matches by value, then return the current TreeNode*
  else if (key == cur->key) { return cur; }
  // Case 3:
  // If the key we're looking for is smaller than the current node's key,
  // then we should look to the left next.
  else if (key < cur->key)  { return _find(key, cur->left); }
  // Case 4:
  // Otherwise, implicitly, the key we're looking for is larger than the
  // current node's key. (We know this because it's not equal and not less.)
  // So we should search to the right next.
  else                      { return _find(key, cur->right); }

}

/**
* insert()
* Inserts `key` and associated `data` into the Dictionary.
*/
template <typename K, typename D>
void Dictionary<K, D>::insert(const K& key, const D& data) {
  TreeNode *& node = _find(key, head_);
  node = new TreeNode(key, data);
}

/**
* remove()
* Removes `key` from the Dictionary. Returns the associated data.
*/
template <typename K, typename D>
const D& Dictionary<K, D>::remove(const K& key) {
  TreeNode*& node = _find(key, head_);
  return _remove(node);
}

template <typename K, typename D>
const D& Dictionary<K, D>::_remove(TreeNode*& node) {
  // Zero child remove:
  if (node->left == nullptr && node->right == nullptr) {
    const D& data = node->data;
    delete node;
    // The slides originally showed "delete(node)".
    // Note that the syntax for "delete" is like an operator, not a function,
    // so it's not necessary to put the () when you use it.
    node = nullptr;
    return data;
  }
  // One-child (left) remove
  else if (node->left != nullptr && node->right == nullptr) {
    const D& data = node->data;
    TreeNode* temp = node;
    node = node->left;
    delete temp;
    return data;
  }
  // One-child (right) remove
  else if (node->left == nullptr && node->right != nullptr) {
    const D& data = node->data;
    TreeNode* temp = node;
    node = node->left;
    delete temp;
    return data;
  }
  // Two-child remove
  else {
    // Find the IOP (in-order predecessor) of the current node.
    TreeNode*& iop = _iop(node);

    // The lecture originally showed the call in this way, but the code has
    // been revised so that the first step to the left happens inside the
    // _iop function:
    // TreeNode*& iop = _iop( node->left ); // old version

    // Since this is the two-child remove case, we know that some in-order
    // predecessor does exist, so the _iop lookup should not have failed.
    if (!iop) {
      throw std::runtime_error("error in two-child remove");
    }

    // Swap the node to remove and the IOP
    _swap_data( node, iop );

    // Remove the new IOP node that was swapped.
    // (Since "node" is now at the old IOP position, it will be removed
    //  by one of the other cases.)
    return _remove( node );
  }
}

// -------

// The implementations for the following functions were not shown in the
// lecture. The _iop function has been revised a little bit, to take one
// step to the left before going to the right. This lets you obtain the
// IOP of the "cur" node when you call _iop(cur) explicitly, instead of
// calling _iop(cur->left) as originally shown. -Eric

template <typename K, typename D>
typename Dictionary<K, D>::TreeNode*& Dictionary<K, D>::_iop(
  TreeNode*& cur) const {

  // get the right-most child of the left subtree of cur

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
  return _rightmost(cur->left);
}

// Get the right-most child of cur using recursion.
// Note that we return a pointer by reference, so it should be an actual
// pointer in our tree that we are returning by reference, not a temporary
// copy of a pointer. This will be true through recursion, as long as we
// initially call _rightmost on a pointer in our tree.
template <typename K, typename D>
typename Dictionary<K, D>::TreeNode*& Dictionary<K, D>::_rightmost(
  TreeNode*& cur) const {

  // Base case 1: If cur is null, then just return it by reference.
  // (Since cur has the value of nullptr, we can check for that result.)
  if (!cur) return cur;

  // Base case 2: So far, we know cur is not null. Now, if cur does not have
  // a right child, then cur is the rightmost, so return cur by reference.
  if (!(cur->right)) return cur;

  // General case: The cur pointer is not null, and it does have a right
  // child, so we should recurse to the right and return whatever we get.
  return _rightmost(cur);
}

template <typename K, typename D>
void Dictionary<K, D>::_swap_data(TreeNode* node1, TreeNode* node2) {
  if (!node1 || !node2) {
    throw std::runtime_error("error: _swap_data used on a null node pointer");
  }
  else {
    std::swap(node1->data, node2->data);
  }
}

