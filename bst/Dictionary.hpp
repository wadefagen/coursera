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
const D& Dictionary<K, D>::find(const K& key) {
  TreeNode*& node = _find(key, head_);
  if (node == nullptr) { throw std::runtime_error("key not found"); }
  return node->data;
}

template <typename K, typename D>
typename Dictionary<K, D>::TreeNode*& Dictionary<K, D>::_find(
  const K& key, TreeNode*& cur) const {

  if      (cur == nullptr)  { return cur; }
  else if (key == cur->key) { return cur; }
  else if (key < cur->key)  { return _find(key, cur->left); }
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
    // Find the IOP (in-order predecessor)
    TreeNode*& iop = _iop( node->left );

    // Swap the node to remove and the IOP
    _swap( node, iop );

    // Remove the new IOP node that was swapped
    return _remove( node );
  }
}

