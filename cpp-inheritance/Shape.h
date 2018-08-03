/**
 * Generic `Shape` class.
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#pragma once

class Shape {
  public:
    Shape();
    Shape(double width);
    double getWidth() const;

  private:
    double width_;
};
