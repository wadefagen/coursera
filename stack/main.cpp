/**
 * Simple use of std::sueue.
 * 
 * @author
 *   Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

#include <iostream>
#include <stack>

int main() {
  // Create a std::stack:
  std::stack<std::string> s;

  // Add several strings to the stack:
  s.push( "Orange" );
  s.push( "Blue" );
  s.push( "Illinois" );

  // Print the front of the stack out and pop it off:
  std::cout << "First pop(): " << s.top() << std::endl;
  s.pop();

  // Add another string and then print ouf the front of the stack:
  s.push( "Illini" );
  std::cout << "Second pop(): " << s.top() << std::endl;

  return 0;
}