/**
 * C++ class for a "stack" of cubes in a Tower of Hanoi puzzle.
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#include "Stack.h"

#include <exception>
#include <iostream>
using std::cout;
using std::endl;

void Stack::push_back(const Cube & cube) {
  // Ensure that we do not push a cube on top of a smaller cube:
  if ( size() > 0 && cube.getLength() > peekTop().getLength() ) {
    std::cerr << "A smaller cube cannot be placed on top of a larger cube." << std::endl;
    std::cerr << "  Tried to add Cube(length=" << cube.getLength() << ")" << std::endl;
    std::cerr << "  Current stack: " << *this << std::endl;

    throw std::runtime_error("A smaller cube cannot be placed on top of a larger cube.");
  }

  cubes_.push_back(cube);
}

Cube Stack::removeTop() {
  Cube cube = peekTop();
  cubes_.pop_back();
  return cube;
}

Cube & Stack::peekTop() {
  return cubes_[cubes_.size() - 1];
}

unsigned Stack::size() const {
  return cubes_.size();
}

std::ostream& operator<<(std::ostream & os, const Stack & stack) {
  for (unsigned i = 0; i < stack.size(); i++) {
    os << stack.cubes_[i].getLength() << " ";
  }
  os << endl;
  return os;
}