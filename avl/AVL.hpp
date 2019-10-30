/**
 * AVL tree auxiliary definitions
 *
 * (Some versions of the lecture videos called this file "AVL.cpp",
 *  but we are renaming it to "AVL.hpp" as it includes templated
 *  implementations and would typically be included as a header.
 *  The line numbers in this file do not exactly match what is shown in the
 *  lecture slides. Please read the comments for further explanation.)
 *
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>, Eric Huber
 */

#pragma once

#include "AVL.h"

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
// Please check out the "binary-tree-traversals" and "bst" example
// directories first for insight on how this works. The "bst" example
// uses a very similar reference-storing design, and I've put additional
// notes there about the design tradeoffs inherent in that.

// ------

template <typename K, typename D>
const D& AVL<K, D>::find(const K& key) {
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
// can refer to the TreeNode type definition that is part of AVL.
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
// AVL<K, D>::TreeNode*&
// That is a pointer to a AVL<K, D>::TreeNode, returned by reference.

template <typename K, typename D>
typename AVL<K, D>::TreeNode*& AVL<K, D>::_find(
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
* Inserts `key` and associated `data` into the AVL tree.
*/
template <typename K, typename D>
void AVL<K, D>::insert(const K& key, const D& data) {
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
* Removes `key` from the AVL tree. Returns the associated data.
*/
template <typename K, typename D>
const D& AVL<K, D>::remove(const K& key) {

  // new wrapper between "remove" and "_remove":
  return _find_and_remove(key, head_);

}

template <typename K, typename D>
const D& AVL<K, D>::_find_and_remove(const K& key, TreeNode*& cur) {

  // We let the "remove" function make the initial call to this one.
  // The basic logic here is similar to _find, but now we want to take
  // advantage of the call stack to ensure balance of everything from
  // the removal point up to the root, after we do the remove.

  if (cur == nullptr) {
    // Key not found
    throw std::runtime_error("error: remove() used on non-existent key");
  }
  else if (key == cur->key) {
    // Found it: REMOVE HERE
    return _remove(cur);
  }
  else if (key < cur->key) {
    // Search left
    const D& d = _find_and_remove(key, cur->left);
    // Ensure balance of this ancestor on the way back up the call stack:
    _ensureBalance(cur);
    return d;
  }
  else {
    // Search right
    const D& d = _find_and_remove(key, cur->right);
    // Ensure balance of this ancestor on the way back up the call stack:
    _ensureBalance(cur);
    return d;
  }

}

// _remove will remove the node pointed to by the argument. Note that this
// will alter the pointer you pass in-place, so you should not reuse the
// pointer variable after calling this function on it. You can't be sure what
// it points to anymore after the function call.
template <typename K, typename D>
const D& AVL<K, D>::_remove(TreeNode*& node) {

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
typename AVL<K, D>::TreeNode*& AVL<K, D>::_swap_nodes(
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

  // We need to swap the heights of the two nodes. For clarity, we'll just
  // do this before changing the pointers. Since the heights are stored as
  // plain values in the nodes, it's easy to swap them.
  std::swap(node1->height, node2->height);

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

template <typename K, typename D>
void AVL<K, D>::_updateHeight(TreeNode*& cur) {
  // If the node is nullptr, then do nothing and return.
  if (!cur) return;
  // Otherwise update the height to be one more than the greater of the
  // two children's heights. The get_height function safely handles the
  // case where a child is just nullptr (no node exists), in which case
  // its implicit height is -1.
  cur->height = 1 + std::max(get_height(cur->left), get_height(cur->right));
}

template <typename K, typename D>
void AVL<K, D>::_ensureBalance(TreeNode*& cur) {

  // Base case for safety: do nothing if cur is nullptr.
  if (!cur) return;

  // Calculate the balance factor for this node:
  const int initial_balance = get_balance_factor(cur);

  // Error checking to make sure our implementation doesn't have a bug
  if (initial_balance < -2 || initial_balance > 2) {
    std::string msg("ERROR: Detected invalid initial balance factor: ");
    msg += std::to_string(initial_balance);
    msg += " ; This should never happen here.";
    throw std::runtime_error(msg);
  }

  // Check if the current node is not in balance,
  // and if it is not, then detect the direction of the imbalance and
  // perform a rotation to correct it.

  if (initial_balance == -2) {
    const int l_balance = get_balance_factor(cur->left);
    if (l_balance == -1) {
      _rotateRight(cur);
    }
    else if (l_balance == 1) {
      _rotateLeftRight(cur);
    }
    else {
      // Error checking
      std::string msg("ERROR: l_balance has unexpected value: ");
      msg += std::to_string(l_balance);
      msg += " ; This should never happen here.";
      throw std::runtime_error(msg);
    }
  }
  else if (initial_balance == 2) {
    const int r_balance = get_balance_factor(cur->right);
    if (r_balance == 1) {
      _rotateLeft(cur);
    }
    else if (r_balance == -1) {
      _rotateRightLeft(cur);
    }
    else {
      // Error checking
      std::string msg("ERROR: r_balance has unexpected value: ");
      msg += std::to_string(r_balance);
      msg += " ; This should never happen here.";
      throw std::runtime_error(msg);
    }
  }

  // If nodes rotated underneath, their heights were updated.
  // We need to update this node's height too.
  _updateHeight(cur);

  // Final error checking:
  // We expect that this should have fixed any imbalance now.
  const int final_balance = get_balance_factor(cur);
  if (final_balance < -1 || final_balance > 1) {
    std::string msg("ERROR: Invalid balance factor after _ensureBalance: ");
    msg += std::to_string(final_balance);
    msg += " ; Something went wrong.";
    throw std::runtime_error(msg);
  }

}

template <typename K, typename D>
void AVL<K, D>::_rotateLeft(TreeNode*& cur) {

  // Here, cur points to the original top-most node that roots the subtree
  // where we will do the left rotation. You might also want to refer
  // to the lecture slides about "generic left rotation".

  // If these error conditions arise, something already went wrong before
  // this function was called.
  if (!cur) {
    throw std::runtime_error("ERROR: _rotateLeft called on nullptr");
  }
  if (!cur->right) {
    throw std::runtime_error("ERROR: _rotateLeft: right child is nullptr");
  }

  // x points to the node that was originally the subtree root.
  // (In the lecture on AVL algorithms, on the diagrams for generic left
  //  rotation, this is shown as node "b".)
  TreeNode* x = cur;
  // y points to the node that was originally the right child.
  // (This was shown as node "c".)
  TreeNode* y = cur->right;
  // z points to the node that was originally the left child of the right child.
  // (This was shown as subtree "t2" in lecture. Understand that this node,
  //  if present, is simply the root of that subtree.)
  TreeNode* z = cur->right->left;

  // Let node x's new right child be the old left child of node y.
  x->right = z;
  // Node y's new left child is x. This puts node y on top.
  y->left = x;
  // Since cur is the original tree node pointer that points to the root
  // of this subtree, we need it to now point to the new root of the
  // subtree, which is node y.
  cur = y;

  // Nodes x and y have changed heights, but z should not have changed height.
  // When we update x and y, it's important to update x first, since it is
  // now the lower of the two, and y's update will depend upon it.
  _updateHeight(x);
  _updateHeight(y);

}

template <typename K, typename D>
void AVL<K, D>::_rotateRight(TreeNode*& cur) {

  // This implementation is a mirror image of _rotateLeft.

  if (!cur) {
    throw std::runtime_error("ERROR: _rotateRight called on nullptr");
  }
  if (!cur->left) {
    throw std::runtime_error("ERROR: _rotateRight: left child is nullptr");
  }

  TreeNode* x = cur;
  TreeNode* y = cur->left;
  TreeNode* z = cur->left->right;

  x->left = z;
  y->right = x;
  cur = y;

  _updateHeight(x);
  _updateHeight(y);

}

template <typename K, typename D>
void AVL<K, D>::_rotateRightLeft(TreeNode*& cur) {

  // Here, cur points to the original top-most node that roots the subtree
  // where we will do the rotation. You might also want to refer to the
  // lecture slides about "generic right-left rotation".

  // We have to be careful about the distinction between making extra
  // temporary copies of pointers to node addresses here vs. operating
  // directly on the captive child pointers that are in the nodes we're
  // dealing with. We need to be sure to actually rewire the node
  // connections that are in the tree by changing the actual pointers,
  // not simply making copies of pointers and doing things to the copies.
  // In this case, all it really means is we must call _rotateLeft directly
  // on the cur pointer that was passed by reference.

  if (!cur) {
    throw std::runtime_error("ERROR: _rotateRightLeft called on nullptr");
  }

  // Perform a right rotation on the right subtree under cur.
  _rotateRight(cur->right);

  // Perform a left rotation on cur.
  _rotateLeft(cur);

}

template <typename K, typename D>
void AVL<K, D>::_rotateLeftRight(TreeNode*& cur) {

  // Similar to _rotateRightLeft

  if (!cur) {
    throw std::runtime_error("ERROR: _rotateLeftRight called on nullptr");
  }

  _rotateLeft(cur->left);

  _rotateRight(cur);

}

template <typename K, typename D>
const D& AVL<K, D>::_iopRemove(TreeNode*& targetNode) {

  if (!targetNode) {
    throw std::runtime_error("ERROR: _iopRemove(TreeNode*& targetNode) called on nullptr");
  }

  // Kick-start the IOP-finding process with the left child of the target,
  // although we ultimately want to find the right-most child of that.
  const D& d = _iopRemove(targetNode, targetNode->left);

  // Make sure we also check the balance of the node that ends up in
  // targetNode's original position, on the way back up the call stack.
  // The rest of the trail up to the root will be re-checked by whichever
  // function called _iopRemove in the first place.
  _ensureBalance(targetNode);

  return d;
}

template <typename K, typename D>
const D& AVL<K, D>::_iopRemove(TreeNode*& targetNode, TreeNode*& iopNode) {

  if (!targetNode) {
    throw std::runtime_error("ERROR: _iopRemove(TreeNode*& targetNode, TreeNode*& iopNode): targetNode is null");
  }

  if (!iopNode) {
    throw std::runtime_error("ERROR: _iopRemove(TreeNode*& targetNode, TreeNode*& iopNode): iopNode is null");
  }

  if (iopNode->right != nullptr) {
    // IoP not found yet. Keep doing deeper recursively:
    const D& d = _iopRemove(targetNode, iopNode->right);
    // After the recursive call has been made, the target node has been
    // removed successfully from the ultimate IOP position.
    // Also, d now has a reference to the removed data.
    // At this line, the "iopNode" pointer is not actually pointing to the
    // IOP at all, but rather it points to the parent (or ancestor) of what
    // had been the actual IOP. We'll call _ensureBalance on these as we
    // return up the call stack, to make sure that any necessary balancing
    // changes or height updates are propagated upwards in the trail of
    // ancestry to the root.
    if (iopNode) {
      _ensureBalance(iopNode);
    }
    return d;
  }
  else {
    // Base case:

    // Found IoP. Swap the location:
    TreeNode*& movedTarget = _swap_nodes(targetNode, iopNode);

    // Remove the swapped node (at IoP's position) and return up the call
    // stack. (What we return is actually a reference to the const data
    // that was removed. This is clear from the return type of _remove.)
    return _remove(movedTarget);
  }

}

// Include the remaining headers in this series of chained header files
#include "AVL-extra.hpp"
