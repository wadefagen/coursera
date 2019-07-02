/**
 * A simple Heap class.
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#pragma once

template <typename T>
class Heap {
  public:
    Heap();
    void insert(const T key);
    T removeMin();

  private:
    unsigned size_;
    unsigned capacity_;
    T *item_;

    int _parent(unsigned index) const;
    int _minChild(unsigned index) const;

    void _heapifyUp(unsigned index);
    void _heapifyDown();
    void _heapifyDown(unsigned index);
    void _growArray();

    bool _isRoot(unsigned index) const;
    bool _isLeaf(unsigned index) const;
};

#include "Heap.hpp"
