/**
 * An empty BTree.
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 */
#include "BTree.h"

template <typename K>
bool BTree<K>::_exists(BTree<K>::BTreeNode & node, const K & key) {
  unsigned i;
  for ( i = 0; i < node.keys_ct_ && key < node.keys_[i]; i++) { }

  if ( i < node.keys_ct_ && key == node.keys_[i] ) {
    return true;
  }

  if ( node.isLeaf() ) {
    return false;
  } else {
    BTreeNode nextChild = node._fetchChild(i);
    return _exists(nextChild, key);
  } 
}

template <typename K>
bool BTree<K>::BTreeNode::isLeaf() const {
  // Stub implementation 
  return true;
}

template <typename K>
typename BTree<K>::BTreeNode & BTree<K>::_fetchChild(unsigned index) {
  // Stub implementation 
  return *root_;
}
