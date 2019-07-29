/**
 * Simple linked-memory, templated list class.
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

// Note: When you define functions within the class definition, you can
// assume that all member variable definitions in the class are in scope,
// regardless of where they are defined. So, we can refer to head_ in the
// List constructor near the top, before the line where head_ is defined.
// More info:
// https://stackoverflow.com/questions/13094898/do-class-functions-variables-have-to-be-declared-before-being-used

#pragma once

template <typename T>
class List {
  public:
    const T & operator[](unsigned index);
    void insertAtFront(const T & data);

    // We define this constructor to make sure that head_ is null-initialized.
    List() : head_(nullptr) { }

    // We define a destructor to delete the memory allocated for the ListNode
    // objects when the List is destroyed.
    ~List() {
      // We'll walk through from the head.
      ListNode *thru = head_;
      while (thru != nullptr) {
        // Copy the address that the "thru" pointer has currently.
        ListNode* toDelete = thru;
        // Step the pointer to the "next" pointer of the current node.
        thru = thru->next;
        // Now, finally, we can delete the toDelete pointer. We could not
        // delete it before we stepped away from it above, because we needed
        // the next pointer information first.
        delete toDelete;
        // We don't strictly need to set toDelete to nullptr here because
        // it goes out of scope immediately after, but remember that you
        // should generally do this after deleting a pointer:
        toDelete = nullptr;
      }
    }

  private:
    class ListNode {
      public:
        const T & data;
        ListNode *next;
        ListNode(const T & data) : data(data), next(nullptr) { }
    };

    ListNode *head_;   /*< Head pointer for our List */
    
    ListNode* _find(const T & data);
};

// Finish including the rest of the header from the additional header file.
// This is just done to spread out our writing among more manageable files.
#include "List.hpp"
