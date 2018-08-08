/**
 * Simple linked-memory, templated list class.
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#pragma once

template <typename T>
class List {
  public:
    const T & operator[](unsigned index);
    void insertAtFront(const T & data);

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

#include "List.hpp"