/**
 * Simple program using a fixed-sized C++ array.
 *
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#include <iostream>

int main() {
  // Create a fixed-sized array of 10 primes:
  int values[10] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };

  // Outputs the 4th (index 3) prime:
  std::cout << values[3] << std::endl;

  return 0;
}
