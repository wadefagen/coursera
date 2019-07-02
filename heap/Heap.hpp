/**
 * A simple Heap class.
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 */
#include "Heap.h"
#include <algorithm>
#include <iostream>

template <typename T>
Heap<T>::Heap() {
  size_ = 0;
  capacity_ = 2;
  item_ = new T[capacity_ + 1];
}


template <typename T>
void Heap<T>::insert(const T key) {
  // Check to ensure there’s space to insert an element
  // ...if not, grow the array
  if ( size_ == capacity_ ) { _growArray(); }

  // Insert the new element at the end of the array
  size_++;
  item_[size_] = key;
  
  // Restore the heap property
  _heapifyUp(size_);

  std::cout << "After Heap<T>::insert(key = " << key << "): ";
  for (unsigned i = 1; i <= size_; i++) {
    std::cout << item_[i] << " ";
  }
  std::cout << std::endl;
}


template <class T>
T Heap<T>::removeMin() {
  // Swap with the last value
  T minValue = item_[1];
  std::swap( item_[1], item_[size_--] );

  // Restore the heap property
  _heapifyDown();

  // Return the minimum value
  return minValue;
}


template <class T>
void Heap<T>::_heapifyUp( unsigned index ) {
  if ( !_isRoot(index) ) {
    if ( item_[index] < item_[ _parent(index) ] ) {
      std::swap( item_[index], item_[ _parent(index) ] );
      _heapifyUp( _parent(index) );
    }
  }
}


template <class T>
void Heap<T>::_heapifyDown() {
  _heapifyDown(1);
}


template <class T>
void Heap<T>::_heapifyDown( unsigned index ) {
  if ( !_isLeaf(index) ) {
    T minChildIndex = _minChild(index);
    if ( item_[index] > item_[minChildIndex] ) {
       std::swap( item_[index], item_[minChildIndex] );
       _heapifyDown( minChildIndex );
    }
  }
}


template <class T>
void Heap<T>::_growArray() {
  T* newItem = new T[(2 * capacity_) + 1];
  for (unsigned i  = 1; i <= capacity_; i++) { newItem[i] = item_[i]; }
  
  capacity_ = 2 * capacity_;
  delete[] item_;
  item_ = newItem;
  std::cerr << "Heap<T>::_growArray() increased array to capacity_ == " << capacity_ << std::endl;
}


template <class T>
bool Heap<T>::_isRoot( unsigned index ) const {
  return (index == 1);
}


template <class T>
bool Heap<T>::_isLeaf( unsigned index ) const {
  // A leaf node has no children, so it is a leaf if the left child (index * 2) is
  // larger than the size of our heap.
  return (index * 2 > size_);
}


template <class T>
int Heap<T>::_parent( unsigned index ) const {
  return index / 2;
}


template <class T>
int Heap<T>::_minChild( unsigned index ) const {
  unsigned left = index * 2;
  unsigned right = (index * 2) + 1;

  if (right > size_) {
    // No right child exists
    return left;
  } else if ( item_[left] <= item_[right] ) {
    return left;
  } else {
    return right;
  }
}