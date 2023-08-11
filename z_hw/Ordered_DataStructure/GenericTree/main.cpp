/**
 * @file main.cpp
 * University of Illinois CS 400, MOOC 2, Week 3: Generic Tree
 *
 * @author Eric Huber
**/

#include <iostream>
#include <string>

#include "GenericTree.h"
#include "GenericTreeExercises.h"

// The exampleTree functions show some examples of how the GenericTree class
// can be used to generate and display a tree.
// Although these functions are defined in this file, listing the function
// prototypes up here before we refer to them in main() allows us to move
// the function bodies to the bottom of the file for clarity.
void exampleTree1();
void exampleTree2();

// Main function: Shows some example output of the provided functions
// and runs a few tests on the ones you implement in the exercises.
int main() {

  exampleTree1();

  exampleTree2();

  treeFactoryTest();

  traversalTest();

  return 0;
}

void exampleTree1() {

  std::cout << std::endl;
  std::cout << "------------------------------" << std::endl;
  std::cout << "EXAMPLE TREE 1" << std::endl  << std::endl;

  // Create a new tree with the root node containing "A".
  GenericTree<std::string> tree("A");

  // Get a pointer to the root node. Automatically detect the
  // type as a pointer to a node.
  auto nodeA = tree.getRootPtr();
  
  // We could write out the entire templated type we expect to be returned,
  // but templated types and inner class types can have very long type
  // names, like this:
  //   GenericTree<std::string>::GenericTreeNode* nodeA = tree.getRootPtr();  
  // It's much easier to use "auto" in these situations, which will
  // automatically detect the type returned.
  
  // Remember that nodeA here is a pointer to a node. Also, addChild returns
  // a pointer to a node. The child nodes created have their pointers stored
  // in the tree itself, but we save copies of the pointers as variables here
  // when we need to refer to them again. For example, this is useful to
  // specify several children branching from the same parent.
  auto nodeB = nodeA->addChild("B");
  nodeB->addChild("C");
  nodeB->addChild("D");
  auto nodeE = nodeA->addChild("E");
  nodeE->addChild("F");
  nodeE->addChild("G");

  std::cout << "Here's a small example tree. (The leftmost branches are displayed\n"
    << " highest in the vertical display format for the terminal.)" << std::endl;
  std::cout << tree << std::endl;

  std::cout << "Debug display of the depth information per node:" << std::endl;
  tree.showDebugMessages = true;
  std::cout << tree << std::endl;

  std::cout << "Now, when the tree goes out of scope and is destroyed, you can see the order\n"
    << " in which the nodes are explored and destroyed for cleanup:" << std::endl;

  return;
}

void exampleTree2() {

  std::cout << std::endl;
  std::cout << "------------------------------" << std::endl;
  std::cout << "EXAMPLE TREE 2" << std::endl  << std::endl;
  std::cout << "Here's a larger tree:" << std::endl << std::endl;

  GenericTree<std::string> tree("X");
  
  // It's possible to clear the entire tree.
  // As a demonstration, let's try clearing the tree and creating a
  // new root with "A" instead.
  tree.clear();
  tree.createRoot("A");

  auto A = tree.getRootPtr();
  // Since addChild returns a pointer to a node, it's possible for us
  // to chain together function calls like this in succession:
  A->addChild("B")->addChild("C");
  auto D = A->addChild("D");
  auto E = D->addChild("E");
  E->addChild("F");
  E->addChild("G")->addChild("H");
  D->addChild("I");
  A->addChild("J");
  auto L = A->addChild("K")->addChild("L");
  L->addChild("M");

  std::cout << tree << std::endl;

  std::cout << "Let's delete the subtrees rooted at D and at L:" << std::endl << std::endl;

  // After deleting the D subtree, the existing D and E pointers
  // won't be safe to dereference anymore. Reset them to nullptr.
  tree.deleteSubtree(D);
  D = E = nullptr;

  // Deleting the L subtree removes nodes L and M.
  // We saved a pointer to L so we need to set it to null for safety.
  tree.deleteSubtree(L);
  L = nullptr;

  std::cout << tree << std::endl;

  std::cout << "Let's try some helper functions to count how many null children are left" << std::endl
    << " over in the tree:" << std::endl << std::endl;

  int nullChildrenCount = countNullChildrenRecursive(tree.getRootPtr());
  std::cout << "Null children counted recursively: " << nullChildrenCount << std::endl;

  nullChildrenCount = countNullChildrenIterative(tree.getRootPtr());
  std::cout << "Null children counted iteratively: " << nullChildrenCount << std::endl;

  std::cout << std::endl;
  std::cout << "Now let's compress the tree to remove the null children pointers:"
    << std::endl << std::endl;
  
  tree.compress();
  std::cout << "After compressing:" << std::endl;
  std::cout << tree << std::endl;
  nullChildrenCount = countNullChildrenIterative(tree.getRootPtr());
  std::cout << "Null children remaining: " << nullChildrenCount << std::endl;

  return;
}

