/**
 * BTree class outline.
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#pragma once

template <typename K>
class BTree {
  public:
    // ...

  private:
    class BTreeNode {
      public:
        K* keys_;
        unsigned keys_ct_;
        bool _isLeaf;

        BTreeNode() : keys_(nullptr), keys_ct_(0), _isLeaf(true) { }
        bool isLeaf() const;
    };
    BTreeNode *root_;

    BTreeNode & _fetchChild(unsigned index);
    bool _exists(BTreeNode & node, const K & key);
    // ...

};

#include "BTree.hpp"