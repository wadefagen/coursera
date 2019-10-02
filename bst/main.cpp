/**
 * Dictionary BST example usage
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>, Eric Huber
 */

#include <string>
#include <iostream>
#include <vector>

#include "Dictionary.h"

int main() {

  // We'll fill a vector with some values matching each index.
  // This is so that we have an existing value to reference with each
  // tree entry in our dictionary. (The lecture video shows literal values
  // being inserted directly, but some compiler versions don't support
  // storing references to temporary literal values.)

  // Initialize a vector 100 elements long, filled with 0s
  const int V_SIZE = 100;
  std::vector<int> v(V_SIZE, 0);
  // Reassign each item to hold a value matching its index
  for (int i=0; i<V_SIZE; i++) {
    v[i] = i;
  }

  Dictionary<int, std::string> t;

  // Let's enable conversion from bool to strings "true" and "false"
  // for the std::cout stream.
  std::cout << std::boolalpha;
  std::cout << "Dictionary empty at the beginning? " << t.empty() << std::endl;

  t.insert(v[37], "thirty seven");
  t.insert(v[19], "nineteen");
  t.insert(v[51], "fifty one");
  t.insert(v[55], "fifty five");
  t.insert(v[4], "four");
  t.insert(v[11], "eleven");
  t.insert(v[20], "twenty");
  t.insert(v[2], "two");

  int v51 = 51;
  std::cout << "t.find(51): " << t.find(v51) << std::endl;

  std::cout << "t.remove(11): " << t.remove(11) << std::endl;



  return 0;
}
