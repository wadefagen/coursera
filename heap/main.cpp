/**
 * Running a simple heap class.
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#include "Heap.h"
#include <iostream>

int main() {
  Heap<int> heap;

  std::cout << " === 10 calls to heap.insert() === " << std::endl;
  heap.insert(4);
  heap.insert(10);
  heap.insert(2);
  heap.insert(22);
  heap.insert(45);
  heap.insert(18);
  heap.insert(-8);
  heap.insert(95);
  heap.insert(13);
  heap.insert(42);

  std::cout << std::endl;
  std::cout << " === 10 calls to heap.removeMin() === " << std::endl;
  std::cout << heap.removeMin() << std::endl;
  std::cout << heap.removeMin() << std::endl;
  std::cout << heap.removeMin() << std::endl;
  std::cout << heap.removeMin() << std::endl;
  std::cout << heap.removeMin() << std::endl;
  std::cout << heap.removeMin() << std::endl;
  std::cout << heap.removeMin() << std::endl;
  std::cout << heap.removeMin() << std::endl;
  std::cout << heap.removeMin() << std::endl;
  std::cout << heap.removeMin() << std::endl;

  return 0;
}