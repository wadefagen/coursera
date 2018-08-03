/**
 * @file HSLAPixel.h
 *
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#pragma once

#include <iostream>
#include <sstream>

namespace uiuc {
  class HSLAPixel {
  public:
    //static uiuc::HSLAPixel ILLINI_ORANGE;
    //static uiuc::HSLAPixel ILLINI_BLUE;



    double h; /**< Hue of the pixel, in degrees [0, 360). */
    double s; /**< Saturation of the pixel, [0, 1]. */
    double l; /**< Luminance of the pixel, [0, 1]. */
    double a; /**< Alpha of the pixel, [0, 1]. */

    /**
     * Constructs a default HSLAPixel.
     * 
     * A default pixel is completely opaque (non-transparent) and white.
     * Opaque implies that the alpha component of the pixel is 1.0.
     * Lower alpha values are (semi-)transparent.
     */
    HSLAPixel();
    
    /**
     * Constructs an opaque HSLAPixel with the given hue, saturation,
     * and luminance values.
     * 
     * @param hue Hue value for the new pixel, in degrees [0, 360).
     * @param saturation Saturation value for the new pixel, [0, 1].
     * @param luminance Luminance value for the new pixel, [0, 1].
     */
    HSLAPixel(double hue, double saturation, double luminance);

    /**
     * Constructs an HSLAPixel with the given hue, saturation,
     * luminance, and alpha values.
     * 
     * @param hue Hue value for the new pixel, in degrees [0, 360).
     * @param saturation Saturation value for the new pixel, [0, 1].
     * @param luminance Luminance value for the new pixel, [0, 1].
     * @param alpha Alpha value for the new pixel, [0, 1].
     */
    HSLAPixel(double hue, double saturation, double luminance, double alpha);




    static HSLAPixel BLUE;
    static HSLAPixel ORANGE;
    static HSLAPixel YELLOW;
    static HSLAPixel PURPLE;

  };
}