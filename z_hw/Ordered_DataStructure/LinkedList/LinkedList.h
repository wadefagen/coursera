
/**
 * @file LinkedList.h
 * University of Illinois CS 400, MOOC 2, Week 1: Linked Lists
 *
 * @author Eric Huber
 *
**/

// We use the pragma once directive to ensure that this header won't
// be included multiple times per compilation unit by mistake.
#pragma once

#include <stdexcept> // for std::runtime_error
#include <iostream> // for std::cerr, std::cout
#include <ostream> // for std::ostream

// LinkedList class: A doubly-linked list. It can be used similarly
// to a double-ended queue or a stack. The nodes are created on the heap
// and connected in a chain by next and prev pointers. The nodes contain
// actual copies of data, not pointers or references to data as seen in
// some examples in this course. Note that because it is doubly-linked
// with prev pointers on each node, you can traverse the list in both
// directions, but you also have to take extra care when inserting nodes.
template <typename T>
class LinkedList {
public:

  // Node type that is particular to the LinkedList<T> type
  class Node {
  public:
    // The next node in the list, or nullptr if this is the last node.
    Node* next;
    // The previous node in the list, or nullptr if this is the first node.
    Node* prev;
    // An actual data item that this node contains.
    T data;
    
    // Default constructor: This lets data be constructed by
    // the default constructor of the T type.
    Node() : next(nullptr), prev(nullptr) {}

    // Argument constructor: Specifies the data that should be copied into
    // the T data member variable.
    Node(const T& dataArg) : next(nullptr), prev(nullptr), data(dataArg) {}

    // Note that although the Node class has its own copy constructor,
    // when copying an actual LinkedList, we must perform manual copying
    // by creating new nodes one at a time with the appropriate data,
    // so that the pointer connections between nodes are actually correct
    // for the copy of the list. Please refer to the copy constructor for
    // the LinkedList class itself.

    // Copy constructor: Constructs this node to be identical to the other,
    // although this one does not exist at the same memory location as other,
    // and so anything that referred to other by address is NOT automatically
    // referring to this node. (Please see the LinkedList class copy constructor.)
    Node(const Node& other) : next(other.next), prev(other.prev),
      data(other.data) {}

    // Copy assignment operator: Please see the note above.
    Node& operator=(const Node& other) {
      next = other.next;
      prev = other.prev;
      data = other.data;
      return *this;
    }

    // Destructor: Although we don't seem to write anything here,
    // the members of the node class will have their own destructors
    // automatically called afterward.
    ~Node() {}

  };

private:

  // The trailing underscore "_" is just a stylistic choice here to signify
  // that these are private member variables of the class instance.

  // The first node in the list, or nullptr if the list is empty.
  Node* head_;
  // The last node in the list, or nullptr if the list is empty.
  Node* tail_;

  // The current number of elements in the list. This should be efficiently
  // updated by any list operations that change the list.
  // (Note: It is NOT efficient to count how many nodes there are each time
  // you want to update this. Instead, assuming this has the correct value
  // when your member functions begin, make an adjustment to it based on how
  // much change has happened in your function.)
  int size_;

public:

  // Note about STL (Standard Template Library) style:
  // Many of the member functions in this class have been named similarly
  // to those of STL containers such as std::list, which you may want to use
  // in the future.

  // (Note about a minor detail here:)
  // We'll reuse this C-style string, just a char array, a few times in diagnostics.
  // It's "static" because it belongs to the entire class type, not to any
  // one instance of the type. For example, there is only one of this string
  // for all LinkedList<int> list objects in memory.
  // We do have to put a definition line in global scope later
  // (below in this file) to let the linker find it.
  static constexpr char LIST_GENERAL_BUG_MESSAGE[] = "[Error] Probable causes: wrong head_ or tail_ pointer, or some next or prev pointer not updated, or wrong size_";

  // This publicly reveals the head and tail pointers of the internal
  // list of nodes that make up our list. This is dangerous! In practice,
  // you would probably not want to give your users such easy access to these
  // private member variables, because someone could cause errors.
  Node* getHeadPtr() { return head_; }
  Node* getTailPtr() { return tail_; }

  // Return a copy of the current size. This can't be used to edit the size_
  // member variable. The size_ variable needs to be maintained efficiently
  // by all member functions of the class. Then, this lookup function allows
  // the list length to be reported in constant time.
  int size() const { return size_; }

  // Returns true if the list is empty. This assumes that the head_ pointer
  // is accurately being maintained by the member functions.
  bool empty() const { return !head_; }

  // Returns a reference to the actual front data item in the list.
  // This can be used to directly change the data in that node.
  // You must make sure that the list is not empty before you call this.
  T& front(){
    if (!head_) {
      throw std::runtime_error("front() called on empty LinkedList");
    }
    else {
      return head_->data;
    }
  }

  // This version of front() is overloaded to be compatible with
  // const instances of this LinkedList type.
  // For example, when you pass a LinkedList<int> by reference to a function
  // that takes an argument of type const LinkedList<int>&, then within that
  // function, the list is treated as const, and this version of front() must
  // exist or front() cannot be called on the object.
  // Please also see the note below in this file about "const correctness".
  const T& front() const {
    if (!head_) {
      throw std::runtime_error("front() called on empty LinkedList");
    }
    else {
      return head_->data;
    }
  }

  // Please see the note about "T& front()" above.
  T& back(){
    if (!tail_) {
      throw std::runtime_error("back() called on empty LinkedList");
    }
    else {
      return tail_->data;
    }
  }

  // Please see the note about "const T& front() const" above.
  const T& back() const {
    if (!tail_) {
      throw std::runtime_error("back() called on empty LinkedList");
    }
    else {
      return tail_->data;
    }
  }

  // Push a copy of the new data item onto the front of the list.
  void pushFront(const T& newData);
  // Push a copy of the new data item onto the back of the list.
  void pushBack(const T& newData);
  // Delete the front item of the list.
  void popFront();
  // Delete the back item of the list.
  void popBack();
  
  // Delete all items in the list, leaving it empty.
  void clear() {
    // As long as there are items left in the list, remove the tail item.
    while (head_) {
      popBack();
    }

    if (0 != size_) throw std::runtime_error(std::string("Error in clear: ") + LIST_GENERAL_BUG_MESSAGE);
  }

  // Two lists are equal if they have the same length
  // and the same data items in each position.
  // This check runs in O(n) time.
  bool equals(const LinkedList<T>& other) const;
  bool operator==(const LinkedList<T>& other) const {
    return equals(other);
  }
  bool operator!=(const LinkedList<T>& other) const {
    return !equals(other);
  }

  // Output a string representation of the list.
  // This requires that the data type T supports stream output itself.
  // This is used by the operator<< overload defined in this file.
  std::ostream& print(std::ostream& os) const;
 
  // Insert a new item to the list in the correct position, assuming the list
  // was previously sorted. The item should be inserted before the earliest
  // item in the list that is greater. (This definition is in a separate
  // file for the homework exercises.)
  void insertOrdered(const T& newData);

  // Checks whether the list is currently sorted in increasing order.
  // This is true if for all adjacent pairs of items A and B in the list: A <= B.
  bool isSorted() const;

  // This returns a sorted copy of the current list. The sort is performed
  // using the insertion sort algorithm that relies on insertOrdered.
  // This is not an efficient operation; insertion sort is O(n^2).
  // We're providing this for sake of comparison and study.
  LinkedList<T> insertionSort() const;
  
  // Create a list of two lists, where the first list contains the first
  // half of the original list, and the second list contains the second half.
  // If the list has an odd number of elements, the first list will be larger
  // by one element. (The lists returned have copies of data and the original
  // list is unaltered.)
  LinkedList<LinkedList<T>> splitHalves() const;
  
  // Returns a list of new lists, where each list contains a single element
  // of the original list. For example, the original list [1, 2, 3] would be
  // returned as [[1],[2],[3]]. The data are copies, and the original list is
  // not altered.
  LinkedList<LinkedList<T>> explode() const;
  
  // Assuming this list instance is currently sorted, and the "other" list is
  // also already sorted, then merge returns a new sorted list containing all
  // of the items from both of the original lists, in linear time.
  // (This definition is in a separate file for the homework exercises.)
  LinkedList<T> merge(const LinkedList<T>& other) const;
  
  // This is a wrapper function that calls one of either mergeSortRecursive
  // or mergeSortIterative.
  LinkedList<T> mergeSort() const;
  
  // The recursive version of the merge sort algorithm, which returns a new
  // list containing the sorted elements of the current list, in O(n log n) time.
  LinkedList<T> mergeSortRecursive() const;

  // The iterative version of the merge sort algorithm, which returns a new
  // list containing the sorted elements of the current list, in O(n log n) time.
  LinkedList<T> mergeSortIterative() const;

  // Default constructor: The list will be empty.
  LinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}
  
  // The copy assignment operator replicates the content of the other list
  // one element at a time so that pointers between nodes will be correct
  // for this copy of the list.
  LinkedList<T>& operator=(const LinkedList<T>& other) {
    // Clear the current list.
    clear();

    // We'll walk along the other list and push copies of its data.
    // (The declaration "const Node*" means a pointer to a const Node,
    //  not a const pointer to Node.)
    const Node* cur = other.head_;
    while (cur) {
      // Push back a copy of this node's data
      pushBack(cur->data);
      // Iterate
      cur = cur->next;
    }

    return *this;
  }
  
  // The copy constructor begins by constructing the default LinkedList,
  // then it does copy assignment from the other list. Please see the
  // definition of the copy assignment operator.
  LinkedList(const LinkedList<T>& other) : LinkedList() {
    *this = other;
  }

  // The destructor calls clear to deallocate all of the nodes.
  ~LinkedList() {
    clear();
  }

  // Checks whether the size has been correctly updated by member functions,
  // and otherwise throws an exception. This is for testing only.
  bool assertCorrectSize() const;
  
  // Checks whether the reverse-direction links in the list, given by
  // the prev pointers on the nodes, are correct. If an error is found,
  // this throws an exception. This is for testing only.
  bool assertPrevLinks() const;

};

// =======================================================================
// Implementation section
// =======================================================================

// Notes about implementing templated member functions:

// Since this is a templated class, there are special considerations
// about where to write the implementation of the member functions.
// The compiler uses templates to generate code at compile time, and so it
// needs to have a complete definition of the template in each compilation
// unit. (Those are the separate .cpp files that are compiled into .o files
// before being linked together into an executable). One way to ensure this
// is to implement the templated functions right here in the header file
// itself, instead of using a cpp file for the class implementation.

// There are other ways to deal with this issue. We could write a separate
// header file with the implementation and include it here with some extra
// preprocessor directives. That would at least keep the implementation in
// a separate file for easy editing, but it wouldn't speed up recompilation
// at all. Another way to deal with it is to place the implementation in
// a cpp file, but then we must explicitly declare ahead of time in that cpp
// file all of the versions of our template that may be used. That makes it
// harder for other people to reuse the library without editing the library
// files.

// Notes about syntax for checking for null pointers:

// When checking whether a pointer is null or not, several pieces of syntax
// have the same effect. If you have a pointer called ptr, both of these
// expressions check if the pointer is null:
//   if (nullptr == ptr)
//   if (!ptr)
// Both of these expressions check if the pointer is NOT null, meaning it
// has any nonzero value:
//   if (nullptr != ptr)
//   if (ptr)
// In addition, because the "->" operator binds more tightly than the "!"
// operator, when we write something like this:
//   if (!head->next)
// That is evaluated like this:
//   if (!(head->next))

// Notes about "const":

// Marking member functions as "const" at the end of their prototype means
// they can not change the class instance. This is useful to keep yourself
// from making a mistake, but also required when a class instance is passed
// as a reference to a const type in a function. In that case, the class
// instance is basically marked "read only", and you can only use its member
// functions that are const.

// When using a pointer to iterate over const objects, you might use a syntax
// like this:
//
// const Node* thisCur = head_;
//
// The type here is pointer to const Node, so the pointer may change to
// point to a different Node, but the actual Nodes that it points to may
// not be edited through it.

// It's important for you to think about what can or should be marked "const"
// and do so as soon as possible as you design. This is called "const
// correctness". https://isocpp.org/wiki/faq/const-correctness

// ---------------------------------------------------------------------

// Operator overload that allows stream output syntax, such as with std::cout
template <typename T>
std::ostream& operator<<(std::ostream& os, const LinkedList<T>& list) {
  return list.print(os);
}

// In some versions of C++ we have to redeclare a constant static member
// at global scope like this to ensure that the linker doesn't give an error.
template <typename T>
constexpr char LinkedList<T>::LIST_GENERAL_BUG_MESSAGE[];

// Push a copy of the new data item onto the front of the list.
template <typename T>
void LinkedList<T>::pushFront(const T& newData) {

  // allocate a new node
  Node* newNode = new Node(newData);

  if (!head_) {
    // If empty, insert as the only item as both head and tail.
    // The Node already has next and prev set to nullptr by default.
    head_ = newNode;
    tail_ = newNode;
  }
  else {
    // Otherwise, add the new item as the head.
    // (We could rewrite this without the temporary variable "oldHead",
    //  but perhaps this way is clearer.)
    Node* oldHead = head_;
    oldHead->prev = newNode;
    newNode->next = oldHead;
    head_ = newNode;
  }

  // update size
  size_++;
}

// Push a copy of the new data item onto the back of the list.
template <typename T>
void LinkedList<T>::pushBack(const T& newData) {

  // allocate a new node
  Node* newNode = new Node(newData);

  if (!head_) {
    // If empty, insert as the only item as both head and tail.
    // The Node already has next and prev set to nullptr by default.
    head_ = newNode;
    tail_ = newNode;
  }
  else {
    // Otherwise, add the new item as the tail.
    // (We could rewrite this without the temporary variable "oldTail",
    //  but perhaps this way is clearer.)
    Node* oldTail = tail_;
    oldTail->next = newNode;
    newNode->prev = oldTail;
    tail_ = newNode;
  }

  // update size
  size_++;
}

// Delete the front item of the list.
template <typename T>
void LinkedList<T>::popFront() {

  // If list is empty, do nothing.
  if (!head_) return;

  // If the next item after the head is null, this is the last and only
  // item in the list.
  if (!head_->next) {
    // deallocate the only item
    delete head_;
    // reset list pointers
    head_ = nullptr;
    tail_ = nullptr;
    // decrease size and make sure it's zero
    size_--;
    if (0 != size_) throw std::runtime_error(std::string("Error in popFront: ") + LIST_GENERAL_BUG_MESSAGE);
    return;
  }

  // Otherwise, we need to remove the head item and set the following item
  // as the new head.

  // Temporarily save the old head pointer
  Node* oldHead = head_;
  // Update head_ to point to the following item.
  head_ = head_->next;
  // Now set the new head_'s previous pointer to null.
  head_->prev = nullptr;
  // Deallocate the old head_ item
  delete oldHead;
  // It's a good practice to set pointers to null after you delete them for safety,
  // even if you don't think you're going to dereference the same pointer again.
  oldHead = nullptr;

  // update size
  size_--;
}

// Delete the back item of the list.
template <typename T>
void LinkedList<T>::popBack() {

  // If list is empty, do nothing.
  if (!head_) return;

  // If the tail item's prev is null, then this is the last and only
  // item in the list.
  if (!tail_->prev) {
    // deallocate the only item
    delete tail_;
    // reset list pointers
    head_ = nullptr;
    tail_ = nullptr;
    // decrease size and make sure it's zero
    size_--;
    if (0 != size_) throw std::runtime_error(std::string("Error in popBack: ") + LIST_GENERAL_BUG_MESSAGE);
    return;
  }

  // Otherwise, we need to remove the tail item and set the preceding item
  // as the new tail.

  // Temporarily save the old tail_ pointer
  Node* oldTail = tail_;
  // Update tail_ to point to the preceding item
  tail_ = tail_->prev;
  // Now set the new tail_'s next pointer to null.
  tail_->next = nullptr;
  // Deallocate the old tail_ item
  delete oldTail;
  // It's a good practice to set pointers to null after you delete them for safety,
  // even if you don't think you're going to dereference the same pointer again.
  oldTail = nullptr;

  // update size
  size_--;
}

// Checks whether the list is currently sorted in increasing order.
// This is true if for all adjacent pairs of items A and B in the list: A <= B.
template <typename T>
bool LinkedList<T>::isSorted() const {
  // Lists of size 0 or 1 are sorted.
  if (size_ < 2) return true;

  // If the list was not empty, then the head pointer should not be null.
  // But you could verify that manually for safety, as always.
  if (!head_) throw std::runtime_error(std::string("Error in isSorted: ") + LIST_GENERAL_BUG_MESSAGE);

  // There are at least two items in the list. We'll compare all adjacent
  // pairs to see if the sorted condition is maintained.
  const Node* prev = nullptr;
  const Node* cur = head_;
  while (cur->next) {
    // Step forward
    prev = cur;
    cur = cur->next;
    if (!(prev->data <= cur->data)) {
      // Previous data was not <= current data, so return false.
      return false;
    }
  }

  // If the function hasn't returned by now, then the sorted condition
  // was maintained throughout the loop.
  return true;
}

// Two lists are equal if they have the same length
// and the same data items in each position.
// This check runs in O(n) time.
template <typename T>
bool LinkedList<T>::equals(const LinkedList<T>& other) const {

  // If the lists are different sizes, they don't have the same contents.
  if (size_ != other.size_) {
    return false;
  }

  // We'll iterate along both lists and check that all items match by value.
  const Node* thisCur = head_;
  const Node* otherCur = other.head_;

  while (thisCur) {
    if (!otherCur) {
      throw std::runtime_error(std::string("Error in equals: ") + "otherCur missing a node or wrong item count");
    }
    if (thisCur->data != otherCur->data) {
      return false;
    }
    thisCur = thisCur->next;
    otherCur = otherCur->next;
  }

  return true;
}

// This returns a sorted copy of the current list. The sort is performed
// using the insertion sort algorithm that relies on insertOrdered.
// This is not an efficient operation; insertion sort is O(n^2).
// We're providing this for sake of comparison and study.
template <typename T>
LinkedList<T> LinkedList<T>::insertionSort() const {
  // Make result list
  LinkedList<T> result;

  // Walk along the original list and insert the items to the result in order.
  const Node* cur = head_;
  while (cur) {
    result.insertOrdered(cur->data);
    cur = cur->next;
  }

  return result;
}

/*
// A different implementation of insertionSort that doesn't use pointers directly
template <typename T>
LinkedList<T> LinkedList<T>::insertionSort() const {
  // Make result list
  LinkedList<T> result;

  // Temporary working copy of original list
  LinkedList<T> temp = *this;

  // Consume the temporary copy and insert items into the result in order
  while (!temp.empty()) {
    result.insertOrdered(temp.front());
    temp.popFront();
  }

  return result;
}
*/

// Output a string representation of the list.
// This requires that the data type T supports stream output itself.
// This is used by the operator<< overload defined in this file.
template <typename T>
std::ostream& LinkedList<T>::print(std::ostream& os) const {
  // List format will be [(1)(2)(3)], etc.
  os << "[";

  // Note that this works correctly for an empty list.
  Node* cur = head_;
  while (cur) {
    os << "(" << cur->data << ")";
    cur = cur->next;
  }

  os << "]";

  return os;
}


// Create a list of two lists, where the first list contains the first
// half of the original list, and the second list contains the second half.
// If the list has an odd number of elements, the first list will be larger
// by one element. (The lists returned have copies of data and the original
// list is unaltered.)
template <typename T>
LinkedList<LinkedList<T>> LinkedList<T>::splitHalves() const {

  // Prepare a list of lists for the result:
  LinkedList<LinkedList<T>> halves;
  // Prepare a working copy of "*this" object to be split:
  LinkedList<T> leftHalf = *this;
  // Prepare an empty right half to fill:
  LinkedList<T> rightHalf;

  // If the original list size is 0 or 1, we don't want to change it.
  // However, for type consistency, we'll still return it as the left "half"
  // paired with the empty right half list.
  if (size_ < 2) {
    halves.pushBack(leftHalf);
    halves.pushBack(rightHalf);
    return halves;
  }

  // (Note about integer division: 
  //  If some positive integer n is odd, then n/2 is the same as (n-1)/2.)

  // If the list size is even, the list will be split evenly in half.
  // If the list size is odd, we'll let the left side of the split
  //  contain 1 extra element.
  int rightHalfLength = size_ / 2;

  for (int i=0; i<rightHalfLength; i++) {
    // Copy a data element from the right end of the left half
    //  to the left end of the right half:
    T dataToCopy = leftHalf.back();
    rightHalf.pushFront(dataToCopy);
    // Remove the element from the left half:
    leftHalf.popBack();
  }

  halves.pushBack(leftHalf);
  halves.pushBack(rightHalf);

  return halves;
}

// Returns a list of new lists, where each list contains a single element
// of the original list. For example, the original list [1, 2, 3] would be
// returned as [[1],[2],[3]]. The data are copies, and the original list is
// not altered.
template <typename T>
LinkedList<LinkedList<T>> LinkedList<T>::explode() const {

  LinkedList<T> workingCopy = *this;

  LinkedList< LinkedList<T> > lists;

  // This could have been done by iterating over the original list with
  // pointers instead, but here we have created a working copy, and as
  // each item is removed from the front, it is put on the back as a
  // singleton list (a list with a single item). We end up with a list
  // of lists, where each item is contained within its own list.
  while (!workingCopy.empty()) {
    LinkedList<T> singletonList;
    singletonList.pushBack(workingCopy.front());
    workingCopy.popFront();
    lists.pushBack(singletonList);
  }

  return lists;
}

// The recursive version of the merge sort algorithm, which returns a new
// list containing the sorted elements of the current list, in O(n log n) time.
template <typename T>
LinkedList<T> LinkedList<T>::mergeSortRecursive() const {

  // The classic recursive definition of mergeSort is elegantly simple
  // to write but the underlying principle is somewhat profound.
  // There are more comments in this function than code.

  // Base case:
  // Recursion needs this as a stopping place from which to return up the call stack.
  // A list of size 0 or 1 is already sorted.
  if (size_ < 2) {
    // Return a copy of the current list.
    return *this;
  }

  // Split this list into a list of two lists (the left and right halves)
  LinkedList<LinkedList<T>> halves = splitHalves();

  // Note that splitHalves usually returns two halves that are definitely
  // both smaller than the original list. The only case where it would not,
  // is in the situation that the input size is smaller than 2, in which
  // case it would return the unchanged list paired with an empty list.
  // However, in this function, we have already handled the base case where
  // the size is smaller than 2. So, we can say for sure that the two
  // halves handled below are both smaller lists than the input was.

  // Make aliases for the left and right halves to edit them in-place,
  // since these are already safe for us to edit as working copies.
  // (If you aren't sure in a situation like this, you could just make
  //  an extra copy instead of trying to edit in-place using references.)
  LinkedList<T>& left = halves.front();
  LinkedList<T>& right = halves.back();

  // Relying on the inductive hypothesis that our algorithm successfully
  // sorts a smaller list than the original input, we recurse on each of
  // the two halves.
  left = left.mergeSortRecursive();
  right = right.mergeSortRecursive();

  // Assume that left and right are both now sorted successfully.

  // The function splits the amount of work in half each time it recurses,
  // and those results are split again, and so on. It takes O(log n) steps
  // before that process reaches the base case. However, at each layer in
  // that descent, there are still n data items being processed, since you
  // have to consider all descendents of both of the first two halves:
  // [1 2 3 4 5 6 7 8 9 10] => One initial list of n items.
  // [1 2 3 4 5] [6 7 8 9 10] => Two lists, but n items still exist in total.
  // [1 2 3] [4 5] [6 7 8] [9 10] ...etc.
  // [1 2] [3] [4 5] [6 7] [8] [9 10]
  // [1] [2] [3] [4] [5] [6] [7] [8] [9] [10]
  // It takes O(log n) layers of splitting before we get lists of single items.

  // Merge the sorted left and right halves to get the sorted list overall.
  return left.merge(right);

  // Concluding notes:
  // Assuming our merge operation runs in O(n), which you will implement as an
  // exercise this week, that's O(n) total merging work on each layer for each
  // of O(log n) layers. This gives the running time for merge sort of O(n log n).

  // Small speedups can be gained by performing the entire mergesort algorithm
  // without making any unnecessary copies of data, or by operating on arrays
  // that remain in cache memory. Many standard library implementations already
  // give you an efficient implementation of this or another O(n log n) sorting
  // algorithm, which might be switched by the system depending on how large
  // your list objects are, or other factors.

  // If you are interested in analysis of algorithms, check out the computer
  // science theory materials maintained by University of Illinois' Prof. Jeff Erickson.
  // http://jeffe.cs.illinois.edu/teaching/algorithms/
}

// The iterative version of the merge sort algorithm, which returns a new
// list containing the sorted elements of the current list, in O(n log n) time.
template <typename T>
LinkedList<T> LinkedList<T>::mergeSortIterative() const {

  // This version of merge sort works by the same principle as the recursive
  // version described elsewhere in this source code file, but the iterative
  // version explicitly tracks which lists need to be split or merged next
  // using a data structure instead of using the recursive call stack.
  // Depending on specific details of the implementation and compiler,
  // one version or the other might be preferable.

  // A list of size 0 or 1 is already sorted.
  if (size_ < 2) {
    // Return a copy of the current list.
    return *this;
  }

  // Iteratively "explode" the original list into a list of lists, where each
  // list contains a single item. We'll use this list of lists as our workQueue,
  // acting as a double-ended queue containing work yet to be done.
  LinkedList< LinkedList<T> > workQueue = explode();

  // The loop invariant condition is that the lists in our queue are always
  // individually sorted. They begin as singleton lists (one item each),
  // which are sorted by definition. As we merge pairs of them, the merge
  // algorithm always produces a sorted result. Therefore we always are
  // handling sorted lists on the queue. Also, as we merge pairs of lists
  // together, the queue size decreases towards containing only a single list.

  // We want to iteratively merge pairs of lists that are about the same size.
  // Take lists two at a time from the front of the workQueue, merge them,
  // and send the result to the back of the workQueue.
  while(workQueue.size() > 1) {
    // Remove two lists from the front of the queue.
    LinkedList<T> left = workQueue.front();
    workQueue.popFront();
    LinkedList<T> right = workQueue.front();
    workQueue.popFront();
    // Merge the two lists.
    LinkedList<T> merged = left.merge(right);
    // Put the result on the back of the queue.
    // (It's important that we put it on the back of the queue, not the front.
    //  Putting it on the back of the queue means we typically merge two small,
    //  equally-sized arrays, resulting in a rounds of work similar to the
    //  recursive version of merge sort. If we tried to move the merged list
    //  to the front, then we would fetch it right back, repeatedly merging the
    //  following single item into one huge array. Can you calculate what the
    //  running time would be then, if we did it the wrong way here?)
    workQueue.pushBack(merged);
  }

  // When the workQueue is reduced to size 1, its only element is the result.
  return workQueue.front();
}

// This is a wrapper function that calls one of either mergeSortRecursive
// or mergeSortIterative.
template <typename T>
LinkedList<T> LinkedList<T>::mergeSort() const {

  // As a wrapper function, this should only call one version of mergeSort
  // or the other and return that result.

  return mergeSortRecursive();
  // return mergeSortIterative();

}

// Checks whether the size has been correctly updated by member functions,
// and otherwise throws an exception. This is for testing only.
template <typename T>
bool LinkedList<T>::assertCorrectSize() const {
  int itemCount = 0;
  const Node* cur = head_;
  while (cur) {
    itemCount++;
    cur = cur->next;
  }
  if (itemCount != size_) throw std::runtime_error(std::string("Error in assertCorrectSize: ") + LIST_GENERAL_BUG_MESSAGE);
  else return true;
}

// Checks whether the reverse-direction links in the list, given by
// the prev pointers on the nodes, are correct. If an error is found,
// this throws an exception. This is for testing only.
template <typename T>
bool LinkedList<T>::assertPrevLinks() const {
  // These should end up being the same list, but we'll build one
  // in the forward direction and the other in the reverse direction.
  LinkedList<const Node*> forwardPtrList;
  LinkedList<const Node*> reversePtrList;
  {
    const Node* cur = head_;
    while (cur) {
      forwardPtrList.pushBack(cur);
      cur = cur->next;
    }
  }
  {
    const Node* cur = tail_;
    while (cur) {
      reversePtrList.pushFront(cur);
      cur = cur->prev;
    }
  }

  if (forwardPtrList == reversePtrList) return true;
  else throw std::runtime_error(std::string("Error in assertPrevLinks: ") + LIST_GENERAL_BUG_MESSAGE);
}

// A different version of assertPrevLinks
// template <typename T>
// bool LinkedList<T>::assertPrevLinks() const {
//   if (head_ == tail_) {
//     if (!head_ && 0==size_) return true;
//     if (head_ && 1==size_) return true;
//     throw std::runtime_error("Error in assertPrevLinks: List size is wrong or tail_ pointer is wrong");
//   }
//   const Node* lastNodeSeen = nullptr;
//   const Node* cur = tail_;
//   while (cur) {
//     lastNodeSeen = cur;
//     cur = cur->prev;
//   }
//   if (head_ != lastNodeSeen) throw std::runtime_error("Error in assertPrevLinks: Some prev pointer may be set incorrectly");
//   return true;
// }


// The rest of the definitions are in the other header file
// (Correct usage of "#pragma once" ensures that it does no harm to write
//  this here, even if both headers are explicitly included in a cpp file.)
#include "LinkedListExercises.h"

