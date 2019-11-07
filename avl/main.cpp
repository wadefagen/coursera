/**
 * AVL example usage
 *
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>, Eric Huber
 */

#include <string>
#include <iostream>
#include <vector>

#include "AVL.h"

int main() {

  // Please see the comments in the bst example project about why we are
  // declaring these variables here in the main() function scope.

  // Initialize a vector 100 elements long, filled with 0 values
  const int V_SIZE = 100;
  std::vector<int> int_storage(V_SIZE, 0);
  // Reassign each item to hold a value matching its index
  for (int i=0; i<V_SIZE; i++) {
    int_storage[i] = i;
  }

  // Create some strings. We'll just make a string version
  // of every integer value that we preallocated.
  std::vector<std::string> string_storage(V_SIZE);
  for (int i=0; i<V_SIZE; i++) {
    // This will associate key i with string "(i)".
    // For example, key 7 will have the data "(7)".
    // We can use the "+" operator to combine std::string objects.
    // The std::string constructor doesn't accept integers by default,
    // so we have to use std::to_string instead for part of this.
    string_storage[i] = std::string("(") + std::to_string(i) + std::string(")");
  }

  // Let's create an inner scoping block here to restrict the lifetime of our
  // tree to be shorter than that of the ints and strings, since the tree
  // will directly reference those items. This way, the ints and strings will
  // be destroyed only after the tree has already gone out of scope at the
  // end, so the tree will be destroyed before the objects that it refers to.
  {
    AVL<int, std::string> t;

    // Let's enable conversion from bool to strings "true" and "false"
    // for the std::cout stream. This is just to show that the empty()
    // member function is working.
    std::cout << std::boolalpha;
    std::cout << "AVL tree empty at the beginning? " << t.empty() << std::endl;

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

    std::cout << "AVL tree empty after insertions? " << t.empty() << std::endl;

    std::cout << "Current tree contents in order:" << std::endl;
    t.printInOrder();
    std::cout << std::endl;

    std::cout << "Using find to show that 51 has been inserted:" << std::endl;
    std::cout << "t.find(51): " << t.find(51) << std::endl;

    std::cout << "Trying to remove some items:" << std::endl;
    std::cout << "t.remove(11): " << t.remove(11) << std::endl;
    std::cout << "t.remove(51): " << t.remove(51) << std::endl;
    std::cout << "t.remove(19): " << t.remove(19) << std::endl;
    std::cout << "t.remove(6): " << t.remove(6) << std::endl;

    std::cout << "Current tree contents in order:" << std::endl;
    t.printInOrder();
    std::cout << std::endl;

    std::cout << "Vertical printout of the tree:" << std::endl;
    t.printVertical();

    // The following "find" query throws an exception when the item is not
    // found, as shown in lecture. Here's a little example of exception
    // handling using the "try...catch" syntax. (If we didn't use this to
    // catch the exception, then the program would just crash to the terminal
    // and display the exception error message there.)
    try {
      std::cout << "Attempting to find a non-existent item, 51: " << std::endl;
      std::cout << "t.find(51): " << t.find(51) << std::endl;
    }
    catch (const std::runtime_error& e) {
      std::cout << "\nCaught exception with error message: " << e.what() << std::endl;
    }

    // Another example
    try {
      std::cout << "Attempting to remove a non-existent item, 99: " << std::endl;
      std::cout << "t.remove(99): " << t.remove(99) << std::endl;
    }
    catch (const std::runtime_error& e) {
      std::cout << "\nCaught exception with error message: " << e.what() << std::endl;
    }

    // Insert a lot of items to test the tree.

    for (int i=60; i<=89; i++) {
      // t.printVertical();
      // std::cerr << "going to insert " << i << std::endl;
      t.insert(int_storage[i], string_storage[i]);
    }
    
    std::cerr << "\nOK\n";
    
    // t.printVertical();


    // End of the block:
    // The AVL tree object will be destroyed now when it goes out of scope.
    // This will trigger all remaining items to be removed.
  }

  // Show that the program exited without crashing. If you try other
  // experiments in the code block above, you may find that they throw
  // uncaught exceptions from our class functions and crash to the terminal.
  std::cout << "Exiting program normally." << std::endl;

  return 0;
}
