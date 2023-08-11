
/**
 * @file GenericTreeExercises.h
 * University of Illinois CS 400, MOOC 2, Week 3: Generic Tree
 * Spring 2019
 *                       STUDENT STARTER FILE
 *
 * @author Eric Huber - University of Illinois staff
 *
**/

/********************************************************************
  NOTE: There are 2 exercises in this file! Please complete them
  both and test your work before handing this file in on Coursera.
  You can jump to the functions that need editing by searching for
  "TODO", but ideally, you should read through this entire code file
  for background information and hints.
 ********************************************************************/

// Prevent the header from being included more than once per cpp file
#pragma once

// It's good to put system headers first, so that if your own libraries
// cause conflicts, the compiler will most likely identify the problem
// as being in your own source code files, where it arises later.
#include <iostream>
#include <string>

// This is the provided GenericTree class. You can study the header file
// liner notes for additional tips and information about the assignment.
#include "GenericTree.h"

/*******************************************************************
  EXERCISE 1: Populate a tree by completing the treeFactory function.
  
  Here is a generic tree that stores ints, that is, an instance of
  GenericTree<int>:

  4
  |
  |_ 8
  |  |
  |  |_ 16
  |  |  |
  |  |  |_ 42
  |  |
  |  |_ 23
  |
  |_ 15

  This is how a GenericTree is printed to standard output in the
  terminal. The root is 4, which has two children: a left child, 8,
  and a right child, 15. 8 has a left child that is 16, and a right
  child that is 23. 16 has a single child that is 42.

  (Recall that for the sake of our terminal printing, "leftmost"
   children are printed first, at the top, and "rightmost" children
   are printed last, at the bottom.)

  You should try to build this tree as a GenericTree<int> in the
  treeFactory function. We'll help you get started below. You can study
  the other provided source files for hints, especially GenericTree.h,
  main.cpp, and this file, and the terminal output you see should resemble
  the diagram shown above.

  ****************************************************************/

// Note about "static" functions at global scope:
// Normally if you have helper functions declared at global scope, you put
// the declarations (function prototypes) in a header file, and the function
// body definitions in a cpp file. That ensures that function bodies don't
// get redefined during the compiling and linking stages, and it's efficient.
// But in some situations you might want or need to put everything in the
// header file, either for readability, or some other reason. (Some libraries
// today are distributed as header files only.)
//   For templated functions, everything usually MUST be in a header file,
// since the compiler refers to the entire definition each time it generates
// specific code later. However, for global-scope helper functions that are
// NOT templated, such as treeFactory, if we insist on defining them in a
// header file, we can put "static" to ensure that the definition is
// restricted to this compilation unit and does not generate conflicts later.

// treeFactory: A GenericTree<int> is passed as input by reference, and this
// function populates it with contents according to the specification given
// above. If the tree passed in already has some contents then they should be
// properly destroyed first. There is no need to return a value because the
// tree is edited in-place by reference.
static void treeFactory(GenericTree<int>& tree) {

  //      *****************************************************
  //                           EXERCISE 1
  //    TODO: Your work here! You should edit this function body!
  //      *****************************************************

  // Edit the function body only. You should leave the function header alone.
  // Build the contents of tree so that it matches the diagram above
  // when you print it out. The main() function runs that test for you.

  tree.clear();
  tree.createRoot(4);
  auto root = tree.getRootPtr();
  auto eight = root->addChild(8);
  root->addChild(15);

  auto sixteen = eight->addChild(16);
  eight->addChild(23);
  sixteen->addChild(42);
}

// treeFactoryTest: This function demonstrates the execution of treeFactory
// and displays a preview of the results in the main function.
// (You do NOT need to edit this function.)
static void treeFactoryTest() {
  std::cout << std::endl;
  std::cout << "------------------------------" << std::endl;
  std::cout << "EXERCISE 1: treeFactoryTest" << std::endl;
  std::cout << "The output should match what you see in the code comments" << std::endl << std::endl;

  GenericTree<int> tree(9999);
  treeFactory(tree);
  std::cout << tree << std::endl;
}

/*******************************************************************
    The second exercise is found at the bottom of this file.
    Please read through for some background discussion.
  ****************************************************************/

// -------------------------------------------------------------------
// Breadth-first vs. Depth-first Search Strategies
// -------------------------------------------------------------------

// In the lecture about tree traversal, the professor made a distinction
// between traversal and search: Traversal aims to walk an entire tree,
// while search is meant to find a specific node. There is a certain
// relationship between the depth-first search strategy and the first
// types of traversal order that the professor discussed: Pre-, in-, and
// post-order reporting during traversal can all be accomplished with a
// very similar implementation strategy to depth-first search. That is,
// the goal is to descend entirely down one branch of the tree before
// descending another. This search strategy is easy to implement recursively,
// but you can also manage to do it iteratively using stack data structures.
// You can see some examples of this in the source code provided with this
// assignment.

// The level-order traversal strategy is related to breadth-first search,
// in that it attempts to finish work on an entire layer of the tree before
// working on the next layer. In contrast with the other methods, it is
// somewhat more natural to implement with a queue. However, it's possible
// to use a combination of data structures, iteration, and recursion to
// achieve certain walks through the tree in certain orders.

// -------------------------------------------------------------------
// Recursive vs. Iterative Design
// -------------------------------------------------------------------

// As a warm up, below is an example of how to implement the same essential
// function two different ways. Both of the countNullChildren functions
// intend to count the number of nodes in a subtree that have null pointers
// taking up space. Sometimes the recursive method is naturally suited to a
// certain problem and produces an elegant solution. However, you do need to
// be careful about memory usage in that case; There is more discussion about
// that in GenericTree.h in the commentary on GenericTree<T>::deleteSubtree.

// Since GenericTree has a vector of subchildren for each node, where each
// node can have many children, it seems natural that you would want to be
// able to iterate over the children nodes, even if you take a recursive
// approach to solving a problem. It's okay to combine recursive and
// iterative techniques in that way. It's also possible to convert
// recursive solutions into iterative ones entirely. Usually that just means
// using a data structure to keep track of what work still needs to be done;
// for example, you could use a queue or stack of node pointers that still
// remain to be explored. Then you would loop over the set of remaining work
// until no work is left to do.

// -------------------------------------------------------------------
// Simplifying Templated Helper Functions at Global Scope
// -------------------------------------------------------------------

// Note that countNullChildrenRecursive is a templated helper function
// defined at global scope, not a member function of our tree class.
// There is no need to overly break down the templated argument type,
// because we can let the template variable stand for the whole type.
// For example, don't try to do this here:
//
// template <typename T>
// int countNullChildrenRecursive(typename GenericTree<T>::TreeNode* subtreeRoot) { ... }
//
// That will cause you problems with compilation and require you to manually
// specify the templated type in more places where you call it. Instead,
// below we simply allow N to stand for the fully-qualified node type:
//
// template <typename N>
// int countNullChildrenRecursive(N* subtreeRoot) { ... }
//
// This makes compilation work easily as long as we only call it on the right
// data types, although it provides less compile-time checking that our input
// types are correct. We have to be careful to pass in only a pointer to some
// TreeNode type. (If we pass in something arbitrary like a pointer to int,
// we'll get other compilation errors, but they won't be as immediately clear
// about what the real problem is.)
//
// When interesting issues like this come up with your design, you have to
// think about whether to use a workaround that may be less clear or less
// safe, or if you should redesign so the issue is avoided.

// -------------------------------------------------------------------

// countNullChildrenRecursive: Given a pointer to a TreeNode, look at the
// subtree that is implicitly rooted at that node, and count how many children
// throughout the tree are null pointers. Here, templated type N should be a
// TreeNode inner type belonging to some type of GenericTree.
// (You do NOT need to edit this function.)
template <typename N>
int countNullChildrenRecursive(N* subtreeRoot) {

  // Base case: If the root of this subtree itself is null, then return 1.
  if (!subtreeRoot) return 1;

  int nullChildrenSum = 0;

  // Note 1: We'll allow our definition of recursion to include a combination
  //  of looping and recursing.
  // Node 2: Since we dereference subtreeRoot below with "->", we had to make
  //  sure that it was not null first. We already handled that base case above.
  // Note 3: Instead of "auto", it also works to write "auto*" or "N*" here
  //  based on how we've set up the template. But you can't use "auto" in the
  //  function arguments list, only inside the function.
  
  // Iterate over the list of children and recurse on each subtree.
  for (auto childPtr : subtreeRoot->childrenPtrs) {

    // Increment the sum by the result of recursing on this child's subtree.
    nullChildrenSum += countNullChildrenRecursive(childPtr);

  }

  // Return the sum.
  return nullChildrenSum;
}

// countNullChildrenIterative: Given a pointer to a TreeNode, look at the
// subtree that is implicitly rooted at that node, and count how many children
// throughout the tree are null pointers. Here, templated type N should be a
// TreeNode inner type belonging to some type of GenericTree.
// (You do NOT need to edit this function.)
template <typename N>
int countNullChildrenIterative(N* subtreeRoot) {

  int nullChildrenSum = 0;

  // Stack of node pointers that we still need to explore (constructed empty)
  std::stack<N*> nodesToExplore;

  // Begin by pushing our subtree root pointer onto the stack
  nodesToExplore.push(subtreeRoot);

  // Loop while there are still nodes to explore
  while (!nodesToExplore.empty()) {

    // Make a copy of the top pointer on the stack, then pop it to decrease the stack
    N* topNode = nodesToExplore.top();
    nodesToExplore.pop();

    if (!topNode) {
      // If the top node pointer is null, then we must not dereference it.
      // Just increment the null counter, then "continue" to jump back to the top of the loop.
      nullChildrenSum++;
      continue;
    }

    // If the node exists, it may have children pointers. Let's iterate
    // through the childrenPtrs vector and push copies of those pointers
    // onto the exploration stack.
    for (auto childPtr : topNode->childrenPtrs) {
      nodesToExplore.push(childPtr);
    }

  }

  // Return the sum.
  return nullChildrenSum;
}

/*******************************************************************
  EXERCISE 2: Implement level-order traversal in the traverseLevels function.

  As discussed above, a level-order traversal is related to the idea of a
  breadth-first traversal. You need to implement the traverseLevels function,
  which takes a tree as input and performs a level-order traversal. Traverse
  the tree one layer at a time, visiting child nodes from left to right, while
  storing copies of the node data in level order as a std::vector. If a null
  child pointer is encountered, no data item should be appended to the results
  for that one pointer. The function should return the std::vector of result
  data in the appropriate order.

  You may implement the body of the function however you want, but you must
  not leak any memory or crash, and your function should work for any
  simple instance of GenericTree<T> (such as GenericTree<int> or
  GenericTree<std::string>). You'll find several good strategies for building
  the function throughout the source files provided with this assignment.

  ****************************************************************/

// traverseLevels: Performs a level-order traversal of the input tree
// and records copies of the data found, in order, in a std::vector,
// which should then be returned.
template <typename T>
std::vector<T> traverseLevels(GenericTree<T>& tree) {

  // This defines a type alias for the appropriate TreeNode dependent type.
  // This might be convenient.
  using TreeNode = typename GenericTree<T>::TreeNode;
  std::queue<TreeNode*> NodeToExplore;

  // Now you can refer to a pointer to a TreeNode in this function like this.
  // TreeNode* someTreeNodePointer = nullptr;

  // This is the results vector you need to fill.
  std::vector<T> results;

  auto rootNodePtr = tree.getRootPtr();
  if (!rootNodePtr) return results;

  //      *****************************************************
  //                           EXERCISE 2
  //    TODO: Your work here! You should edit this function body!
  //      *****************************************************

  // Perform a level-order traversal and record the data of the nodes in
  // the results vector. They should be placed in the vector in level order.
  // Remember that you can add a copy of an item to the back of a std::vector
  // with the .push_back() member function.
  NodeToExplore.push(rootNodePtr);

  while (!NodeToExplore.empty())
  {
    TreeNode *currNode = NodeToExplore.front();
    NodeToExplore.pop();
    results.push_back(currNode->data);

    for (auto child : currNode->childrenPtrs)
    {
      NodeToExplore.push(child);
    }
  }

  return results;
}

// traversalTest: Runs some tests with your traverseLevels function and
// displays comparison output. (You do NOT need to edit this function.)
static void traversalTest() {
  std::cout << std::endl;
  std::cout << "------------------------------" << std::endl;
  std::cout << "EXERCISE 2: traversalTest" << std::endl;
  std::cout << "Testing your traverseLevels function" << std::endl  << std::endl;

  {
    // This is the tree from exampleTree1() in main.cpp
    std::cout << "[Test 1] Expected output:" << std::endl
      << "A B E C D F G" << std::endl;
    GenericTree<std::string> tree1("A");
    auto nodeA = tree1.getRootPtr();
    auto nodeB = nodeA->addChild("B");
    nodeB->addChild("C");
    nodeB->addChild("D");
    auto nodeE = nodeA->addChild("E");
    nodeE->addChild("F");
    nodeE->addChild("G");
    std::vector<std::string> tree1_results = traverseLevels(tree1);
    std::cout << "Your traverseLevels output:" << std::endl;
    for (auto result : tree1_results) {
      std::cout << result << " ";
    }
    std::cout << std::endl << std::endl;
  }
  
  {
    // This is the tree from exampleTree2() in main.cpp
    std::cout << "[Test 2] Expected output:" << std::endl
      << "A B D J K C E I L F G M H" << std::endl;
    GenericTree<std::string> tree2("A");
    auto A = tree2.getRootPtr();
    A->addChild("B")->addChild("C");
    auto D = A->addChild("D");
    auto E = D->addChild("E");
    E->addChild("F");
    E->addChild("G")->addChild("H");
    D->addChild("I");
    A->addChild("J");
    auto L = A->addChild("K")->addChild("L");
    L->addChild("M");
    std::vector<std::string> tree2_results = traverseLevels(tree2);
    std::cout << "Your traverseLevels output:" << std::endl;
    for (auto result : tree2_results) {
      std::cout << result << " ";
    }
    std::cout << std::endl << std::endl;
  }

  {
    // This is the tree you should have built for the first part of this
    // assignment above, with treeFactory.
    std::cout << "[Test 3] Expected output:" << std::endl
      << "4 8 15 16 23 42" << std::endl;
    GenericTree<int> tree3(9999);
    treeFactory(tree3);
    std::vector<int> tree3_results = traverseLevels(tree3);
    std::cout << "Your traverseLevels output:" << std::endl;
    for (auto result : tree3_results) {
      std::cout << result << " ";
    }
    std::cout << std::endl << std::endl;
  }

}

