/**
 * BinaryTree class outline.
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#pragma once

template <typename T>
class BinaryTree {
  public:
    // ...

  private:
    class TreeNode {
      public:
        T & data;
        TreeNode *left, *right;
        TreeNode(T & data) : data(data), left(nullptr), right(nullptr) { }
    };

    TreeNode *root_;
};

#include "BinaryTree.hpp"

