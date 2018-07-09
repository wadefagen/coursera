/**
 * Simple C++ class for representing a Cube (with constructors).
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#include "Cube.h"
#include <iostream>

using std::cout;
using std::endl;

namespace uiuc {  
  Cube::Cube() {
    length_ = 1;
    cout << "Created $1 (default)" << endl;
  }

  Cube::Cube(double length) {
    length_ = length;
    cout << "Created $" << getVolume() << endl;
  }

  Cube::Cube(const Cube & obj) {
    length_ = obj.length_;
    cout << "Created $" << getVolume() << " via copy" << endl;
  }

  Cube::~Cube() {
    cout << "Destroyed $" << getVolume() << endl;
  }

  Cube & Cube::operator=(const Cube & obj) {
    cout << "Transformed $" << getVolume() << "-> $" << obj.getVolume() << endl;
    length_ = obj.length_;
    return *this;
  }



  double Cube::getVolume() const {
    return length_ * length_ * length_;
  }

  double Cube::getSurfaceArea() const {
    return 6 * length_ * length_;
  }

  void Cube::setLength(double length) {
    length_ = length;
  }
}
