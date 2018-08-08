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

  std::cout << "Inserting element 3 at front..." << std::endl; 
  list.insertAtFront(3);
  std::cout << "list[0]: " << list[0] << std::endl;

  std::cout << "Inserting element 30 at front..." << std::endl; 
  list.insertAtFront(30);
  std::cout << "list[0]: " << list[0] << std::endl;
  std::cout << "list[1]: " << list[1] << std::endl;

  return 0;
}