/**
 * @file main.cpp
 * University of Illinois CS 400, MOOC 2, Week 1: Linked Lists
 *
 * @author Eric Huber
**/

#include <iostream>
#include <string>

#include "LinkedList.h"
#include "LinkedListExercises.h"

// -----------------------------------------------------------------------
// Function prototypes: When global functions are defined later in the
// compilation unit than they are called, then at least these function
// prototype header lines must be written before the call.

// Some informal unit tests. Defined at the bottom of main.cpp
void informalTests();
// Helper function used by informalTests. Defined at the bottom of main.cpp
void errorReaction(std::string msg);

// -----------------------------------------------------------------------

// main program: You can do whatever you want to test your code here.
// See also the test suite that you can build with "make test".
int main() {
  informalTests();
  return 0;
}

// ------------------------------------------------------------------------

// Choose what to do if an informal test has a big problem.
void errorReaction(std::string msg) {
  // If you want to throw an exception and stop the program immediately:
  // throw std::runtime_error(msg);
  // If you want to display a warning and keep going:
  std::cout << std::endl << "WARNING: " << msg << std::endl << std::endl;;
}

// These tests aren't very exhaustive of the student exercise code.
// They are more of a sanity check on the LinkedList class itself.
void informalTests() {

  std::cout << "Running a few informal tests. Feel free to study these or edit them." << std::endl;
  std::cout << "The unit tests in the tests/ subdirectory are much more exhaustive!" << std::endl;
  std::cout << "You can run those by doing: make clean && make test && ./test" << std::endl;
  std::cout << "Remember that when you're ready to submit, you should do: make zip" << std::endl;
  std::cout << "That will package the zip file properly for submission." << std::endl;

  {
    std::cout << std::endl << "Testing splitHalves():" << std::endl;
    LinkedList<int> l;
    l.pushBack(5);
    l.pushBack(6);
    l.pushBack(7);
    l.pushBack(8);
    l.pushBack(9);
    std::cout << "List: " << l << std::endl;
    std::cout << "Size: " << l.size() << std::endl;
    LinkedList<LinkedList<int>> halves = l.splitHalves();
    std::cout << "Front half: " << halves.front()
      << " Back half: " << halves.back() << std::endl;

    LinkedList<int> expectedFrontHalf;
    expectedFrontHalf.pushBack(5);
    expectedFrontHalf.pushBack(6);
    expectedFrontHalf.pushBack(7);
    std::cout << "Expected front half: " << expectedFrontHalf << std::endl;
    if (expectedFrontHalf != halves.front()) errorReaction("front half is wrong");
    LinkedList<int> expectedBackHalf;
    expectedBackHalf.pushBack(8);
    expectedBackHalf.pushBack(9);
    std::cout << "Expected back half: " << expectedBackHalf << std::endl;
    if (expectedBackHalf != halves.back()) errorReaction("back half is wrong");
  }

  {
    std::cout << std::endl << "Testing explode() and iterating with pointers:" << std::endl;
    LinkedList<int> l;
    l.pushBack(5);
    l.pushBack(6);
    l.pushBack(7);
    l.pushBack(8);
    l.pushBack(9);
    std::cout << "Original list: " << l << std::endl;

    auto lists = l.explode();
    std::cout << "Exploded list of lists:" << std::endl;
    std::cout << lists << std::endl;

    std::cout << "Iterating through lists using pointers:" << std::endl;
    auto nodePtr = lists.getHeadPtr();
    while (nodePtr) {
      auto list = nodePtr->data;
      if (list.size() != 1) errorReaction("exploded list item should have size 1");
      std::cout << "List: " << list << std::endl;
      // std::cout << "Size: " << list.size() << std::endl << std::endl;
      nodePtr = nodePtr->next;
    }

    // You can also avoid dealing with pointers entirely by using the
    // push and pop interface to cycle through the list contents. (This is somewhat
    // less efficient because it creates and destroys copies of the data throughout
    // the process of shuffling the list.)
    std::cout << "Fake iteration done by popping and pushing items:" << std::endl;
    for (int i=0; i<lists.size(); i++) {
      auto list = lists.front();
      std::cout << "List: " << list << std::endl;
      // std::cout << "Size: " << list.size() << std::endl << std::endl;
      lists.popFront();
      lists.pushBack(list);
    }
  }

  {
    std::cout << std::endl << "Testing insertOrdered:" << std::endl;

    LinkedList<int> expected;
    expected.pushBack(-1);
    expected.pushBack(4);
    expected.pushBack(5);
    expected.pushBack(6);
    expected.pushBack(7);
    expected.pushBack(8);
    expected.pushBack(9);
    expected.pushBack(10);

    LinkedList<int> l;
    l.pushBack(5);
    l.pushBack(6);
    l.pushBack(8);
    l.pushBack(9);
    std::cout << "List: " << l << std::endl;
    l.insertOrdered(7);
    l.insertOrdered(4);
    l.insertOrdered(-1);
    l.insertOrdered(10);
    std::cout << "Inserting items now." << std::endl;
    std::cout << "List: " << l << std::endl;
    std::cout << "Expected: " << expected << std::endl;
    if (l != expected) errorReaction("wrong result");

    std::cout << "Testing copy constructor:" << std::endl;
    LinkedList<int> l2(l);
    std::cout << "Copy: " << l2 << std::endl;
    std::cout << "Expected: " << expected << std::endl;
    if (l2 != expected) errorReaction("wrong result");

    std::cout << "Checking equivalency:" << std::boolalpha << (l == l2) << std::endl;
  }

  {
    std::cout << std::endl << "Testing insertionSort:" << std::endl;
    LinkedList<int> l;
    l.pushBack(10);
    l.pushBack(4);
    l.pushBack(7);
    l.pushBack(5);
    l.pushBack(8);
    l.pushBack(6);
    l.pushBack(-1);
    l.pushBack(9);

    std::cout << "(Before) List: " << l << std::endl;
    auto l2 = l.insertionSort();
    std::cout << "(After) List: " << l2 << std::endl;
    std::cout << "isSorted: " << std::boolalpha << l2.isSorted() << std::endl;
    
    LinkedList<int> expected;
    expected.pushBack(-1);
    expected.pushBack(4);
    expected.pushBack(5);
    expected.pushBack(6);
    expected.pushBack(7);
    expected.pushBack(8);
    expected.pushBack(9);
    expected.pushBack(10);

    std::cout << "Expected: " << expected << std::endl;

    if (l2 != expected) errorReaction("Bug in insertionSort (check insertOrdered first)");
    if (l2 == expected && !l2.isSorted()) errorReaction("Bug in isSorted");
  }

  {
    std::cout << std::endl << "Testing merge():" << std::endl;
    LinkedList<int> l;
    l.pushBack(1);
    l.pushBack(3);
    l.pushBack(5);
    LinkedList<int> r;
    r.pushBack(-1);
    r.pushBack(2);
    r.pushBack(10);
    r.pushBack(20);

    LinkedList<int> expected;
    expected.pushBack(-1);
    expected.pushBack(1);
    expected.pushBack(2);
    expected.pushBack(3);
    expected.pushBack(5);
    expected.pushBack(10);
    expected.pushBack(20);

    std::cout << "Left List: " << l << "  ";
    std::cout << "Right List: " << r << std::endl;
    auto merged = l.merge(r);
    std::cout << "Merged: " << merged << std::endl;
    std::cout << "Expected: " << expected << std::endl;
    if (merged != expected) errorReaction("Bug in merge");
  }

  {
    std::cout << std::endl << "Testing mergeSortRecursive:" << std::endl;
    LinkedList<int> l;
    l.pushBack(10);
    l.pushBack(4);
    l.pushBack(7);
    l.pushBack(5);
    l.pushBack(8);
    l.pushBack(6);
    l.pushBack(-1);
    l.pushBack(9);

    LinkedList<int> expected;
    expected.pushBack(-1);
    expected.pushBack(4);
    expected.pushBack(5);
    expected.pushBack(6);
    expected.pushBack(7);
    expected.pushBack(8);
    expected.pushBack(9);
    expected.pushBack(10);

    std::cout << "(Before) List: " << l << std::endl;
    l = l.mergeSortRecursive();
    std::cout << "(After) List: " << l << std::endl;
    std::cout << "isSorted: " << std::boolalpha << l.isSorted() << std::endl;
    std::cout << "Expected: " << expected << std::endl;
    if (l != expected) errorReaction("Bug in mergeSortRecursive (check merge first)");
  }

  {
    std::cout << std::endl << "Testing mergeSortIterative:" << std::endl;
    LinkedList<int> l;
    l.pushBack(10);
    l.pushBack(4);
    l.pushBack(-1);
    l.pushBack(5);
    l.pushBack(8);
    l.pushBack(6);
    l.pushBack(7);
    l.pushBack(9);

    LinkedList<int> expected;
    expected.pushBack(-1);
    expected.pushBack(4);
    expected.pushBack(5);
    expected.pushBack(6);
    expected.pushBack(7);
    expected.pushBack(8);
    expected.pushBack(9);
    expected.pushBack(10);

    std::cout << "(Before) List: " << l << std::endl;
    l = l.mergeSortIterative();
    std::cout << "(After) List: " << l << std::endl;
    std::cout << "isSorted: " << std::boolalpha << l.isSorted() << std::endl;
    std::cout << "Expected: " << expected << std::endl;
    if (l != expected) errorReaction("Bug in mergeSortIterative (check merge first)");
  }

  std::cout << "\n\nInformal tests finished without crashing, but the output may have been incorrect."
    << "\nPlease scroll to the top of the output to read the diagnostics." << std::endl;

}

