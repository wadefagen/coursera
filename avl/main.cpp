/**
 * AVL example usage
 *
 * @author Wade Fagen-Ulmschneider <waf@illinois.edu>
 * @author Eric Huber
 */

#include <string>
#include <iostream>
#include <vector>

#include "AVL.h"

// Please see the introductory notes in AVL.h before you study this file.

int main() {

  // We'll allocate this many items contiguously in memory externally to the
  // AVL tree, which the tree will be able to reference in its nodes.
  // We require at least 100 here to run the basic examples shown, and at
  // least 1000 to run the extended unit tests for debugging.
  const int V_SIZE = 1000;

  if (V_SIZE < 100) {
    throw std::runtime_error("Please set V_SIZE to at least 100");
  }

  // Initialize a vector V_SIZE elements long, filled with 0 values
  std::vector<int> int_storage(V_SIZE, 0);
  // Reassign each item to hold a value matching its index
  for (int i=0; i<V_SIZE; i++) {
    int_storage[i] = i;
  }

  // Create some strings. We'll just make a string version
  // of every integer value that we preallocated.
  std::vector<std::string> string_storage(V_SIZE);
  for (int i=0; i<V_SIZE; i++) {
    // This will associate key i with string "i".
    // For example, key 7 will have the data "7".
    // We have to use std::to_string to convert the ints to strings.
    string_storage[i] = std::to_string(i);
  }

  // Let's create an inner scoping block here to restrict the lifetime of our
  // tree to be shorter than that of the ints and strings, since the tree
  // will directly reference those items. This way, the ints and strings will
  // be destroyed only after the tree has already gone out of scope at the
  // end, so the tree will be destroyed before the objects that it refers to.
  {
    std::cout << "\nCreating AVL tree now..." << std::endl;
    AVL<int, std::string> t;

    // Let's enable conversion from bool to strings "true" and "false"
    // for the std::cout stream. This is just to show that the empty()
    // member function is working.
    std::cout << std::boolalpha;
    const bool empty_at_beginning = t.empty();
    std::cout << "AVL tree empty at the beginning? " << empty_at_beginning << std::endl;
    if (!empty_at_beginning) {
      throw std::runtime_error("Error: empty() should have been true at the beginning");
    }

    std::cout << "Inserting items..." << std::endl;

    // Note that we insert items here by reference to the actual items in the
    // vector "v" where they reside. The [] indexing operator for std::vector
    // returns a reference.
    t.insert(int_storage[37], string_storage[37]);
    t.insert(int_storage[19], string_storage[19]);
    t.insert(int_storage[51], string_storage[51]);
    t.insert(int_storage[55], string_storage[55]);
    t.insert(int_storage[4], string_storage[4]);
    t.insert(int_storage[11], string_storage[11]);
    t.insert(int_storage[20], string_storage[20]);
    t.insert(int_storage[2], string_storage[2]);
    t.insert(int_storage[3], string_storage[3]);
    t.insert(int_storage[5], string_storage[5]);
    t.insert(int_storage[6], string_storage[6]);
    t.insert(int_storage[7], string_storage[7]);

    // When we call find and remove below, we don't need to use v[] and we
    // can specify a number just by a literal value. That's because our
    // member functions are designed to find a key that matches the argument
    // by value equivalence, not by memory identity.

    const bool empty_after_insertions = t.empty();
    std::cout << "AVL tree empty after insertions? " << empty_after_insertions << std::endl;
    if (empty_after_insertions) {
      throw std::runtime_error("Error: empty() should have been false after insertions");
    }

    std::cout << "\nCurrent tree contents in order:" << std::endl;
    t.printInOrder();
    std::cout << std::endl;

    std::cout << "\nUsing find to show that 51 has been inserted:" << std::endl;
    std::cout << "t.find(51): " << t.find(51) << std::endl;

    std::cout << "\nTrying to remove some items:" << std::endl;
    std::cout << "t.remove(11): " << t.remove(11) << std::endl;
    std::cout << "t.remove(51): " << t.remove(51) << std::endl;
    std::cout << "t.remove(19): " << t.remove(19) << std::endl;
    std::cout << "t.remove(6): " << t.remove(6) << std::endl;

    std::cout << "\nCurrent tree contents in order:" << std::endl;
    t.printInOrder();
    std::cout << std::endl;

    std::cout << "\nVertical printout of the tree:" << std::endl;
    t.printVertical();

    // The following "find" query throws an exception when the item is not
    // found, as shown in lecture. Here's a little example of exception
    // handling using the "try...catch" syntax. (If we didn't use this to
    // catch the exception, then the program would just crash to the terminal
    // and display the exception error message there.)
    try {
      std::cout << std::endl;
      std::cout << "Attempting to find a non-existent item, 51: " << std::endl;
      std::cout << "t.find(51): " << t.find(51);
      std::cout << std::endl;
    }
    catch (const std::runtime_error& e) {
      std::cout << "(OK) Caught example exception with the following message:" << std::endl
        << "\"" << e.what() << "\"" << std::endl;
    }

    // Another example
    try {
      std::cout << std::endl;
      std::cout << "Attempting to remove a non-existent item, 99: " << std::endl;
      std::cout << "t.remove(99): " << t.remove(99);
      std::cout << std::endl;
    }
    catch (const std::runtime_error& e) {
      std::cout << "(OK) Caught example exception with the following message:" << std::endl
        << "\"" << e.what() << "\"" << std::endl;
    }

    if (V_SIZE >= 1000) {
      // --- Begin extended tests ---

      std::cout << "\n --- Beginning extended tests ---\n"
        << "  (Many items will be inserted and removed silently...)" << std::endl;

      // Clear the tree contents (remove all nodes)
      t.clear_tree();

      // Insert a lot of items to test the tree.
      for (int i=10; i<=900; i++) {
        t.insert(int_storage[i], string_storage[i]);
      }
      std::cout << "\nInsert test OK\n";

      // Remove a lot of items to test the tree.
      for (int i=10; i<=900; i+=7) {
        t.remove(i);
      }
      for (int i=900; i>=10; i-=3) {
        // The "contains" check helps avoid throwing an exception when
        // items are not found.
        if (t.contains(i)) {
          t.remove(i);
        }
      }
      std::cout << "\nRemove test OK\n";

      // Insert items again and remove some again
      for (int i=10; i<=899; i+=2) {
        // Only insert items that aren't already there.
        // Let's try to mix up the order of the insertions here
        // a little bit by inserting two adjacent numbers in order
        // and then inserting something towards the other end of the
        // number range.
        if (!t.contains(i)) {
          t.insert(int_storage[i], string_storage[i]);
        }
        if (!t.contains(i+1)) {
          t.insert(int_storage[i+1], string_storage[i+1]);
        }
        // "j" will be an index towards the other side of the number range
        int j = 900 - i + 10;
        if (!t.contains(j)) {
          t.insert(int_storage[j], string_storage[j]);
        }

      }
      for (int i=10; i<=900; i+=7) {
        if (t.contains(i)) {
          t.remove(i);
        }
        // "j" will be an index towards the other side of the number range
        int j = 900 - i + 10;
        if (t.contains(j)) {
          t.remove(j);
        }
      }
      std::cout << "\n --- End of extended tests ---" << std::endl;
      
      // --- End of extended tests ---      
    }
    else {
      std::cout << "\n(V_SIZE is set too small. skipping extended unit tests.)" << std::endl;
    }

    // We're about to exit the block scope that the AVL tree object was
    // created in, on the stack. The AVL tree object will be destroyed now
    // when it goes out of scope. This will trigger all remaining items to
    // be removed.
    std::cout << "\nAVL tree will go out of scope and be destroyed now."
      << "\n(All nodes will be removed...)\n";
  }

  // Show that the program exited without crashing. If you try other
  // experiments in the code block above, you may find that they throw
  // uncaught exceptions from our class functions and crash to the terminal.
  std::cout << "\nSUCCESS - The program is exiting normally." << std::endl;

  return 0;
}
