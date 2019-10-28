/**
 * AVL tree auxiliary definitions, part 2
 *
 * Extra definitions for debugging, etc.
 *
 * @author
 *   Eric Huber
 */

#pragma once

#include <stack>

#include "AVL.hpp"

template <typename K, typename D>
void AVL<K, D>::printVertical() const {

  std::stack<TreeNode*> node_stack;
  std::stack<int> margin_stack;

  node_stack.push(head_);
  margin_stack.push(0);

  while (!node_stack.empty()) {

    TreeNode* n = node_stack.top();
    node_stack.pop();

    int margin_level = margin_stack.top();
    margin_stack.pop();

    for (int i=0; i < margin_level; i++) {
      std::cout << " ";
    }

    if (margin_level > 0) {
      std::cout << "|- ";
    }
    else {
      std::cout << ". ";
    }

    if (n) {

      // Only push child markers if at least one child exists.
      // This makes the output clearer.
      if (n->left || n->right) {
        node_stack.push(n->right);
        margin_stack.push(margin_level+1);
        node_stack.push(n->left);
        margin_stack.push(margin_level+1);
      }

      std::cout << "[" << n->key << ": " << n->data << "]" << std::endl;
    }
    else {
      std::cout << "[]" << std::endl;
    }

  }

}
