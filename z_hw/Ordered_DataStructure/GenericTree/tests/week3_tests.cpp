
// University of Illinois CS 400, MOOC 2, Week 3: Generic Tree
// Author: Eric Huber, University of Illinois staff
// Autograder based on Zephyr test runner by Prof. Wade Fagen-Ulmschneider and the CS 225 Course Staff
// Based on Catch2 unit testing framework

#include <cstdlib>
#include <sstream>

#include "../uiuc/catch/catch.hpp"

#include "../GenericTree.h"
#include "../GenericTreeExercises.h"


TEST_CASE("Displaying manual test output", "[weight=0]") {
  treeFactoryTest();
  traversalTest();
}

TEST_CASE("Testing treeFactory preliminaries", "[weight=1]") {
  GenericTree<int> tree(9999);
  treeFactory(tree);
  auto root = tree.getRootPtr();
  SECTION("Root should not be null") {
    REQUIRE(nullptr != root);
  }
  SECTION("Root data should remove the previous setting") {
    REQUIRE(root);
    REQUIRE(9999 != root->data);
  }
  SECTION("Root data should be 4") {
    REQUIRE(root);
    REQUIRE(4 == root->data);
  }
  SECTION("Deepest data should be 42") {
    REQUIRE(root);
    REQUIRE(root->childrenPtrs.at(0));
    REQUIRE(root->childrenPtrs.at(0)->childrenPtrs.at(0));
    REQUIRE(root->childrenPtrs.at(0)->childrenPtrs.at(0)->childrenPtrs.at(0));
    REQUIRE(42 == root->childrenPtrs.at(0)->childrenPtrs.at(0)->childrenPtrs.at(0)->data);
  }
}

TEST_CASE("Testing treeFactory further", "[weight=1]") {

  std::string exemplarTreeStr = "4\n";
  exemplarTreeStr += "|\n";
  exemplarTreeStr += "|_ 8\n";
  exemplarTreeStr += "|  |\n";
  exemplarTreeStr += "|  |_ 16\n";
  exemplarTreeStr += "|  |  |\n";
  exemplarTreeStr += "|  |  |_ 42\n";
  exemplarTreeStr += "|  |\n";
  exemplarTreeStr += "|  |_ 23\n";
  exemplarTreeStr += "|\n";
  exemplarTreeStr += "|_ 15\n";
  GenericTree<int> tree(9999);
  treeFactory(tree);
  std::stringstream output;
  output << tree;
  SECTION("Trees should match") {
    REQUIRE(output.str() == exemplarTreeStr);
  }
}

TEST_CASE("Testing traverseLevels", "[weight=2]"){
  // This is the tree from exampleTree2() in main.cpp
  // std::cout << "[Test 2] Expected output:" << std::endl
  //   << "A B D J K C E I L F G M H" << std::endl;
  std::string expected_traversal = "A B D J K C E I L F G M H ";
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
  // std::cout << "Your traverseLevels output:" << std::endl;
  std::stringstream outstream;
  for (auto result : tree2_results) {
    outstream << result << " ";
  }
  auto student_traversal = outstream.str();
  SECTION("Should do correct traversal on tree from exampleTree2()") {
    REQUIRE(expected_traversal == student_traversal);
  }
}

