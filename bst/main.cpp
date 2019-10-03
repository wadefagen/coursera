/**
 * Dictionary BST example usage
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>, Eric Huber
 */

#include <string>
#include <iostream>
#include <vector>

#include "Dictionary.h"

int main() {

  // We'll fill a vector with some values matching each index.
  // This is so that we have an existing value to reference with each
  // tree entry in our dictionary. The lecture video shows literal values
  // being inserted directly, but some compiler versions don't support
  // storing references to temporary literal values. In general, it's a good
  // idea to only store references to memory that you have guaranteed will
  // be valid for some duration of the code; in this case, we can ensure
  // that the vector of actual data items will stay in scope for the rest
  // of the main function, as long as our tree will exist.

  // Initialize a vector 100 elements long, filled with 0s
  const int V_SIZE = 100;
  std::vector<int> v(V_SIZE, 0);
  // Reassign each item to hold a value matching its index
  for (int i=0; i<V_SIZE; i++) {
    v[i] = i;
  }

  // Let's create an inner scoping block here to restrict the lifetime
  // of our tree to be shorter than that of the data vector. The vector
  // will be destroyed only after the tree has already gone out of scope
  // and been destroyed at the end.
  {
    Dictionary<int, std::string> t;

    // Let's enable conversion from bool to strings "true" and "false"
    // for the std::cout stream. This is just to show that the empty()
    // member function is working.
    std::cout << std::boolalpha;
    std::cout << "Dictionary empty at the beginning? " << t.empty() << std::endl;

    std::cout << "Inserting items..." << std::endl;

    t.insert(v[37], "thirty seven");
    t.insert(v[19], "nineteen");
    t.insert(v[51], "fifty one");
    t.insert(v[55], "fifty five");
    t.insert(v[4], "four");
    t.insert(v[11], "eleven");
    t.insert(v[20], "twenty");
    t.insert(v[2], "two");

    std::cout << "Dictionary empty after insertions? " << t.empty() << std::endl;

    std::cout << "t.find(51): " << t.find(51) << std::endl;

    std::cout << "t.remove(11): " << t.remove(11) << " (zero child remove)" << std::endl;
    std::cout << "t.remove(51): " << t.remove(51) << " (one child remove)" << std::endl;
    std::cout << "t.remove(19): " << t.remove(19) << " (two child remove)" << std::endl;

    try {
      // The following line throws an exception when the item is not found.
      // If we didn't use the "try...catch" mechanism, this would just cause
      // the program to crash and display the exception error message anyway.
      std::cout << "t.find(51): " << t.find(51) << std::endl;
    }
    catch (std::exception& e) {
      std::cout << "\nCaught exception with error message:" << std::endl;
      std::cout << e.what() << std::endl;
    }
  }

  // Show that the program exited without crashing. If you try other
  // experiments in the code block above, you may find that they throw
  // uncaught exceptions from our class functions and crash to the terminal.
  std::cout << "Exiting program normally." << std::endl;

  return 0;
}
