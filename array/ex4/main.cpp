/**
 * Calculating the memory seperation of elements in a std::vector.
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#include <iostream>
#include <vector>
#include "../Cube.h"

using uiuc::Cube;

int main() {
  std::vector<Cube> cubes{ Cube(11), Cube(42), Cube(400) };

  // Examine capacity:  
  std::cout << "Initial Capacity: " << cubes.capacity() << std::endl;
  cubes.push_back( Cube(800) );
  std::cout << "Size after adding: " << cubes.size() << std::endl;
  std::cout << "Capacity after adding: " << cubes.capacity() << std::endl;

  // Using pointer arithmetic, ask the computer to calculate
  // the offset from the beginning of the array to [2]:
  int offset = (long)&(cubes[2]) - (long)&(cubes[0]);
  std::cout << "Memory separation: " << offset << std::endl;

  // Find a specific `target` cube in the array:
  Cube target = Cube(400);
  for (unsigned i = 0; i < cubes.size(); i++) {
    if (target == cubes[i]) {
      std::cout << "Found target at [" << i << "]" << std::endl;
    }
  }

  return 0;
}
