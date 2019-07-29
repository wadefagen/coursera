/**
 * Simple C++ main using a linked-memory list class.
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#include "List.h"
#include <iostream>

int main() {
  List<int> list;

  // We'll be able to store const references to these objects in the List
  // because these constants on the stack exist in the same function scope
  // that lasts until the end of main().
  const int item_a = 3;
  const int item_b = 30;

  std::cout << "Inserting element 3 at front..." << std::endl; 
  list.insertAtFront(item_a);
  std::cout << "list[0]: " << list[0] << std::endl;

  std::cout << "Inserting element 30 at front..." << std::endl; 
  list.insertAtFront(item_b);
  std::cout << "list[0]: " << list[0] << std::endl;
  std::cout << "list[1]: " << list[1] << std::endl;

  return 0;
}