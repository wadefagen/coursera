/**
 * Examples of ValueBinaryTree usage and some tree traversals
 *
 * @author
 *   Eric Huber, Wade Fagen-Ulmschneider <waf@illinois.edu>
 */

// It's a good idea to include any standard libraries first, before your
// own header files, not afterwards:
#include <string>
// That way if your own header files have bugs that cause incompatibility,
// the compiler will usually point to the bug in your own header, not in the
// standard library.
#include "ValueBinaryTree.h"

// There may be a few exceptions to this in the real world, if you are using
// a library that is designed to transform itself based on the types you've
// already defined. We use the Catch2 testing framework sometimes, and it is
// like that.

int main() {

  // We'll just write some inner scoping blocks to clean up the stack memory
  // after each example usage. Variables we declare on the stack within a
  // block scope will only exist until the block ends. This is similar to
  // what would happen if we wrote a separate example-running function for
  // each example and called them here.

  // Example of traversals with a complete tree of integers
  {
    // The constructors we've set up for our ValueBinaryTree type let us
    // specify a complete binary tree's contents based on a vector of input
    // values. We can initialize a vector in-place using the {} initializer
    // notation. Then, that vector will be passed as an argument to the
    // constructor for the tree we're creating. (The compiler will be able to
    // guess that it's a std::vector<int> we are trying to initialize this
    // way, because it can detect that we're trying to pass an argument to
    // the constructor and match this with one of the defined constructors
    // for our tree type that is compatible.)
    ValueBinaryTree<int> seven_tree({1,2,3,4,5,6,7});

    // We'll get a pointer to the root node of the tree.
    // See the note in ValueBinaryTree.h about why this is "unsafe".
    auto rootPtr = seven_tree.unsafe_getRootPtr();

    std::cout << "Example of pre-order traversal with a complete tree: " << std::endl;

    // Do the pre-order traversal now, which outputs the values:
    seven_tree.preOrder(rootPtr);
    // Output line breaks for neatness and clarity on the terminal:
    std::cout << std::endl << std::endl;

    // Expected output:
    // 1 2 4 5 3 6 7

    std::cout << "Example of in-order traversal with a complete tree: " << std::endl;

    seven_tree.inOrder(rootPtr);
    std::cout << std::endl << std::endl;

    // Expected output:
    // 4 2 5 1 6 3 7

    std::cout << "Example of post-order traversal with a complete tree: " << std::endl;

    seven_tree.postOrder(rootPtr);
    std::cout << std::endl << std::endl;

    // Expected output:
    // 4 5 2 6 7 3 1
  }

  // -------

  // What about a level-order traversal? Read through the other source code
  // files for ValueBinaryTree to see some notes about that.

  // -------

  // Example from lecture: Traversals on an algebraic syntax tree.
  {
    // For convenience, we'll start by constructing a complete tree with most
    // of the contents we want.
    ValueBinaryTree<std::string> algebra_tree({"+", "-", "*", "a", "/", "d", "e"});

    // Now we'll get the root pointer and manually add the rest of the nodes.
    auto rootPtr = algebra_tree.unsafe_getRootPtr();
    // Navigate to the "/" node and save that pointer for reuse.
    auto slashPtr = rootPtr->left->right;

    // When we need to name the TreeNode type that goes with our current
    // std::string version of ValueBinaryTree, we have to give the fully-
    // qualified name of the type like this:
    slashPtr->left = new ValueBinaryTree<std::string>::TreeNode("b");

    // But we can also define a type synonym with "using ... = ...",
    // which makes our code easier to read. This is just an alias (name)
    // for the same type.
    using StringNode = ValueBinaryTree<std::string>::TreeNode;

    slashPtr->right = new StringNode("c");

    // The pre-order and post-order traversal output won't make sense.
    // The syntax tree in this example can be "read out" properly by doing
    // an in-order traversal.

    std::cout << "Pre-order traversal of algebraic syntax tree:" << std::endl;
    std::cout << " (This output won't make sense...)" << std::endl;
    algebra_tree.preOrder(rootPtr);
    std::cout << std::endl << std::endl;

    // Expected output:
    // + - a / b c * d e 

    std::cout << "In-order traversal of algebraic syntax tree:" << std::endl;
    std::cout << " (This one should make sense.)" << std::endl;
    algebra_tree.inOrder(rootPtr);
    std::cout << std::endl << std::endl;

    // Expected output:
    // a - b / c + d * e

    std::cout << "Post-order traversal of algebraic syntax tree:" << std::endl;
    std::cout << " (This output won't make sense...)" << std::endl;
    algebra_tree.postOrder(rootPtr);
    std::cout << std::endl << std::endl;

    // Expected output:
    // a b c / - d e * +

    // (We used the level-order traversal of this tree when we constructed it
    // based on its contents level by level. Read ValueBinaryTree.hpp to see
    // how that works.)
  }

  return 0;
}

