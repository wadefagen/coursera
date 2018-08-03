/**
 * @file HSLAPixel.cpp
 * Implementation of the HSLAPixel class for use in with the PNG library.
 *
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#include "HSLAPixel.h"
#include <cmath>
#include <iostream>
using namespace std;

namespace uiuc {
  HSLAPixel HSLAPixel::BLUE = HSLAPixel(240, 1, 0.5);
  HSLAPixel HSLAPixel::ORANGE = HSLAPixel(30, 1, 0.5);
  HSLAPixel HSLAPixel::YELLOW = HSLAPixel(60, 1, 0.5);
  HSLAPixel HSLAPixel::PURPLE = HSLAPixel(270, 1, 0.5);
  
  HSLAPixel::HSLAPixel() {
    h = 0;
    s = 0;
    l = 1.0;
    a = 1.0;
  }

  HSLAPixel::HSLAPixel(double hue, double saturation, double luminance) {
    h = hue;
    s = saturation;
    l = luminance;
    a = 1.0;
  }

  HSLAPixel::HSLAPixel(double hue, double saturation, double luminance, double alpha) {
    h = hue;
    s = saturation;
    l = luminance;
    a = alpha;
  }
}
