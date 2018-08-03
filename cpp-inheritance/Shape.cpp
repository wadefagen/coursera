#include "Shape.h"

Shape::Shape() : Shape(1) {
  // Nothing.
}

Shape::Shape(double width) : width_(width) {
  // Nothing.
}

double Shape::getWidth() const {
  return width_;
}
