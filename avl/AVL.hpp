/**
 * AVL tree auxiliary definitions
 *
 * (Some versions of the lecture videos called this file "AVL.cpp",
 *  but we are renaming it to "AVL.hpp" as it includes templated
 *  implementations and would typically be included as a header.)
 *
 * @author Wade Fagen-Ulmschneider <waf@illinois.edu>
 * @author Eric Huber
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

// Please first see the introductory notes in the AVL.h file.

// ------

template <typename K, typename D>
const D& AVL<K, D>::find(const K& key) {
  // Find the key in the tree starting at the head.
  // If found, we receive the tree's actual stored pointer to that node
  //   through return-by-reference.
  // If not found, then the node returned has a value of nullptr.
  TreeNode*& node = _find(key, head_);
  if (node == nullptr) { throw std::runtime_error("error in find(): key not found"); }
  // We found the node, so return the actual data there, by reference.
  return node->data;
}

template <typename K, typename D>
bool AVL<K, D>::contains(const K& key) {
  // This is just like "find" but when the item is not found, we just return
  // false instead of throwing an exception. When found, return true.

  TreeNode*& node = _find(key, head_);

  // Carefully note what the following return statement does:
  // If the item was found, then (node != nullptr) is true, so just return
  // the evaluated comparison, which has the value true. If the item was
  // not found, then just the opposite: the expression will evaluate to
  // false. So again, simply returning the expression itself gives the
  // correct value: false.

  // This will be "true" when found, and "false" when not found:
  return node != nullptr;
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
  // error.
  //   (Note: The "cur" we return in this case is equal to nullptr, but
  // it's important to write "return cur;" and not "return nullptr;" since
  // this function returns by reference. We want to return the exact pointer
  // that we found at this position (returning it by reference), even if it
  // is a nullptr that will be trigger a "not found" error in the caller
  // function that we return to. We should not return a reference to the
  // "nullptr" literal; trying to use references to temporary values or
  // literal constants can cause new issues depending on which version of
  // the language (and compiler) you're using. We can avoid those problems
  // by being sure to return references to actual pointers that are owned
  // by our tree.)
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

  // This function will begin a recursion process that will find the place
  // to insert the new node, insert it, and then rebalance the tree as needed
  // while returning up the recursive call stack to this point.
  _find_and_insert(key, data, head_);

  // Run some optional brute-force debugging checks. This could be
  // deactivated for better efficiency. (The constant that toggles whether
  // this is run or not is defined and initialized in the class definition
  // itself, but this line of code could also just be commented out.)
  runDebuggingChecks();

}

template <typename K, typename D>
void AVL<K, D>::_find_and_insert(const K& key, const D& data, TreeNode*& cur) {

  // We let the "insert" function make the initial call to this one.
  // The basic logic here is similar to _find, but now we want to take
  // advantage of the call stack to ensure balance of everything from
  // the insertion point up to the root, after we do the insertion.

  if (cur == nullptr) {
    // In this case we've found the empty child position where we should
    // insert the item.
    cur = new TreeNode(key, data);
    // Note that we always insert the new node as a leaf, so it is already
    // balanced. It has height 0 by default (because of the node class
    // constructor), and it has no children, so there's no need to call
    // the "ensure balance" function on it. We just return up the call
    // stack now.
    return;
  }
  else if (key == cur->key) {
    // If we found a match for the key, then the key already exists,
    // so report an error. (For the sake of this example, let's disallow
    // duplicates. We could also do something nicer than this, like remove
    // the old key and then insert the new item to replace it.)
    throw std::runtime_error("error in insert(): key already exists");
  }
  else if (key < cur->key) {
    // Search left and insert
    _find_and_insert(key, data, cur->left);
    // On the way back up, ensure the balance of this node
    _ensureBalance(cur);
    return;
  }
  else {
    // Search right and insert
    _find_and_insert(key, data, cur->right);
    // On the way back up, ensure the balance of this node
    _ensureBalance(cur);
    return;
  }

}

/**
* remove()
* Removes `key` from the AVL tree. Returns the associated data.
*/
template <typename K, typename D>
const D& AVL<K, D>::remove(const K& key) {

  // Begin the recursion process with this function that will find the
  // node to remove, remove it, and then rebalance the tree as needed
  // while it returns.
  const D& d = _find_and_remove(key, head_);

  // Run some optional brute-force debugging checks. This could be
  // deactivated for better efficiency. (The constant that toggles whether
  // this is run or not is defined and initialized in the class definition
  // itself, but this line of code could also just be commented out.)
  runDebuggingChecks();

  return d;
}

template <typename K, typename D>
const D& AVL<K, D>::_find_and_remove(const K& key, TreeNode*& cur) {

  // We let the "remove" function make the initial call to this one.
  // The basic logic here is similar to _find, but now we want to take
  // advantage of the call stack to ensure balance of everything from
  // the removal point up to the root, after we do the remove.

  if (cur == nullptr) {
    // Key not found
    throw std::runtime_error("error in remove(): key not found");
  }
  else if (key == cur->key) {
    // Found the node to remove; remove it recursively and return the data.
    // (There's no need to "ensure balance" of the node being removed.)
    return _remove(cur);
  }
  else if (key < cur->key) {
    // Search left and remove
    const D& d = _find_and_remove(key, cur->left);
    // Ensure balance and update height of this ancestor
    // on the way back up the call stack:
    _ensureBalance(cur);
    return d;
  }
  else {
    // Search right and remove
    const D& d = _find_and_remove(key, cur->right);
    // Ensure balance and update height of this ancestor
    // on the way back up the call stack:
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
  if (!node) { throw std::runtime_error("error: _remove() used on nullptr"); }

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
    // "temp" is a temporary local variable here, but as a good habit, let's
    // set it to nullptr anyway rather than trying to over-optimize.
    temp = nullptr;

    // Note: The "node" pointer that we've now obtained here is the lower
    // node to the left. There is no need to call "ensure balance" or
    // "update height" on this pointer now, because the height and balance
    // of the subtree rooted at that node, which depend on what is beneath,
    // have not changed.

    return data;
  }
  // One-child (right) remove
  else if (node->left == nullptr && node->right != nullptr) {
    // This case is symmetric to the previous case.
    const D& data = node->data;
    TreeNode* temp = node;
    node = node->right;
    delete temp;
    temp = nullptr;

    return data;
  }
  // Two-child remove
  else {
    // When the node being deleted has two children,
    // we have to be very careful.

    // This wrapper function will correctly remove the specified node
    // after swapping it with its IOP. There is a lot of complexity here.
    // See the other function implementations for comments. After this,
    // it returns a reference to the data of the removed node.
    return _iopRemove(node);
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
  cur->height = 1 + std::max(_get_height(cur->left), _get_height(cur->right));
}

template <typename K, typename D>
void AVL<K, D>::_ensureBalance(TreeNode*& cur) {

  // Base case for safety: do nothing if cur is nullptr.
  if (!cur) return;

  // Calculate the balance factor for this node:
  const int initial_balance = _get_balance_factor(cur);

  // Error checking to make sure our implementation doesn't have a bug
  if (initial_balance < -2 || initial_balance > 2) {
    std::string msg("ERROR: Detected invalid initial balance factor: ");
    msg += std::to_string(initial_balance);
    msg += " ; This should never happen here.";
    std::cerr << "key: " << cur->key << " data: " << cur->data << std::endl;
    std::cerr << "nl: " << cur->left->key << " nr: " << cur->right->key << std::endl;
    std::cerr << "hl: " << _get_height(cur->left) << " hr: " << _get_height(cur->right) << std::endl;
    throw std::runtime_error(msg);
  }

  // Check if the current node is not in balance. If it is not, then we check
  // the balance in the direction of the imbalance, and use that information
  // to choose the correct rotation to apply.

  if (initial_balance == -2) {
    const int l_balance = _get_balance_factor(cur->left);
    if (l_balance == -1 || l_balance == 0) {
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
    const int r_balance = _get_balance_factor(cur->right);
    if (r_balance == 1 || r_balance == 0) {
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

  // If nodes rotated anywhere underneath, their heights were updated.
  // Then we need to update this node's height too.
  // (Note that we do this regardless of whether the above conditional
  // section did a rotation here or not. That's because another function
  // may have called "_ensureBalance" as part of a recursive process,
  // assuming that at the very least, this function would update the height
  // of cur. That would be necessary if nodes were rotated further beneath
  // this point in the tree. So to fulfill these assumptions, we make sure
  // to update the height of cur here.)
  _updateHeight(cur);

  // Final error checking:
  // We expect that this should have fixed any imbalance now.
  const int final_balance = _get_balance_factor(cur);
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

  // Here, the target node means the node we intend to remove.

  if (!targetNode) {
    throw std::runtime_error("ERROR: _iopRemove(TreeNode*& targetNode) called on nullptr");
  }

  // Kick-start the IOP-finding process with the left child of the target,
  // although we ultimately want to find the right-most child of that.
  // The last argument "true" signals that it is the initial call we are
  // making to that version of the recursive function on the way down.
  const D& d = _iopRemove(targetNode, targetNode->left, true);

  // A lot of things happened during the recursive call that we just made.
  // There are two nodes that still need to get their balance checked here:
  // First, we need to check the balance of the first node to the left, if
  // any node still exists in that position. This is because the base case
  // in the other version of _iopRemove doesn't check it there. After that,
  // we also check the balance of "targetNode", which is now the node that
  // ended up in targetNode's original upper position. The rest of the nodes
  // above this, that are on the trail up to the root, must be re-checked by
  // whichever function had called _iopRemove in the first place.
  if (targetNode->left) {
    _ensureBalance(targetNode->left);
  }
  _ensureBalance(targetNode);

  return d;
}

template <typename K, typename D>
const D& AVL<K, D>::_iopRemove(TreeNode*& targetNode, TreeNode*& iopAncestor, bool isInitialCall) {

  // Here, iopAncestor is pointing to either the most recent ancestor node
  // on the path as we head down to the actual IOP, or it is the IOP itself.
  // (That is the base case that we will reach.)

  // "isInitialCall" is set to true the first time we call this function on
  // the way down, and otherwise, we set it to false (when we recurse here).

  if (!targetNode) {
    throw std::runtime_error("ERROR: _iopRemove(TreeNode*& targetNode, TreeNode*& iopAncestor): targetNode is null");
  }

  if (!iopAncestor) {
    throw std::runtime_error("ERROR: _iopRemove(TreeNode*& targetNode, TreeNode*& iopAncestor): iopAncestor is null");
  }

  if (iopAncestor->right != nullptr) {
    // General case: IoP not found yet.
    // Keep doing deeper recursively, eventually removing the target after
    // a swap, and getting the data back. We have to pass "false" for the
    // next "isInitialCall" argument now.
    const D& d = _iopRemove(targetNode, iopAncestor->right, false);

    // After the recursive call has been made, the target node has been
    // removed successfully from the ultimate IOP position.
    // Also, d now has a reference to the removed data.

    // In many cases, the "iopAncestor" pointer is now still pointing to
    // one of the ancestors of the actual IOP node position. That is
    // convenient to us here, because on the way back up from the recursion
    // process, we'll call _ensureBalance on these to make sure that any
    // necessary balancing changes or height updates are propagated upwards
    // in the trail of ancestry to the root. However, if the current node
    // was the first node to the left at the beginning of the process, then
    // its immediate parent was just involved in a swap with the IOP! This
    // is a problem for us here because we no longer have a safe way to
    // directly change the pointer that points to this node. To get around
    // this problem, we've passed an argument that tells whether it's the
    // initial call to this version of _iopRemove or not, on the way down.
    // If it is the initial call, then this was the first node to the left,
    // so we can't call _ensureBalance on the "iopAncestor" pointer here:
    // that pointer is no longer valid! Instead, we'll skip that here, and
    // the outer _iopRemove wrapper function will do the job of ensuring the
    // balance of its left child at the end.

    if (!isInitialCall) {
      // When it's not the initial call on the path traversing right, then
      // this node's parent was not swapped, so the iopAncestor pointer is
      // still valid here. We can ensure the balance of this node now, while
      // we go back up the call stack.

      // The version originally shown on the lecture slides had checked for
      // nullptr here. In this version, we shouldn't really have to, since
      // _ensureBalance now does this also, but also because if it had been
      // a swap between the targetNode and the IOP here that caused the
      // ancestor to no longer exist, then we would be in the base case branch
      // of this function now, not here. In any case, it's okay to have a
      // slightly redundant safety check, with almost no performance cost at
      // all. We don't want to manually over-optimize this logic and possibly
      // make a mistake.
      if (iopAncestor) {
        _ensureBalance(iopAncestor);
      }
    }

    return d;
  }
  else {
    // Base case: Here, iopAncestor points to the actual IOP.

    // We found the IoP. Swap the nodes by location, altering the pointers:
    // After swapping positions, we get back a reference to the updated
    // pointer to the targeted node. Meanwhile, "targetNode" will end up
    // pointing to the hoisted node that replaced the target's position.
    // We need to remember that when we return up the call stack, since
    // targetNode was passed by reference.
    TreeNode*& movedTarget = _swap_nodes(targetNode, iopAncestor);

    // Let's think carefully about what just happened.
    // If the target node was not the direct parent of the IOP, then the
    // swap was fairly simple, and now the pointer "targetNode" is actually
    // pointing to the node that used to be the IOP, which is now raised
    // to the higher position. So we'll want to make sure that the other
    // _iopRemove wrapper function calls _ensureBalance on "targetNode"
    // after we get out of the recursive call stack there. We can't do it
    // right now, because first we want to make sure _ensureBalance gets
    // called on the other nodes in the lineage on the way back up.
    // The current function takes care of that as it returns through the
    // other conditional branch (the general case).

    // If the target node was the direct parent of the IOP, then this is
    // still true. "targetNode" is now pointing to the raised node.
    // The only complication is that the "iopAncestor" pointer is now
    // pointing to something we aren't sure of, but we won't refer to it
    // again here, so it doesn't matter. That's why we got back the
    // "movedTarget" pointer from _swap_nodes instead, so we always know
    // the correct pointer to remove from the lower position.

    // Since the moved target is now in the lower position that was the
    // IOP, we know that removing it from that position will either be a
    // zero-child remove or a one-child remove now. It can't be another
    // two-child remove because then we must not have found the IOP yet.
    // Since it's not going to be another two-child remove, we don't need
    // to worry about _iopRemove being called yet again here.

    // Remove the swapped node (at IoP's position) and return up the call
    // stack. What we return is actually a reference to the const data
    // that was removed.
    const D& d = _remove(movedTarget);
    return d;
  }

}

// Include the remaining headers in this series of related header files
#include "AVL-extra.hpp"
