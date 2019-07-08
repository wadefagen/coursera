#include "Game.h"
#include "Stack.h"
#include "uiuc/Cube.h"
#include "uiuc/HSLAPixel.h"

#include <iostream>
using std::cout;
using std::endl;

// Game default constructor
Game::Game() {
  // The initial default game state has three stacks four cubes.
  
  // Create the three empty stacks:
  for (int i = 0; i < 3; i++) {
    Stack stackOfCubes;
    stacks_.push_back( stackOfCubes );
  }

  // Create the four cubes, placing each on the [0]th stack:
  // - A blue cube of length=4, on the bottom
  // - A orange cube of length=3, on top of the blue cube
  // - A purple cube of length=2, on top of the orange cube
  // - A yellow cube of length=1 at the very top
  Cube blue(4, uiuc::HSLAPixel::BLUE);
  stacks_[0].push_back(blue);

  Cube orange(3, uiuc::HSLAPixel::ORANGE);
  stacks_[0].push_back(orange);

  Cube purple(2, uiuc::HSLAPixel::PURPLE);
  stacks_[0].push_back(purple);

  Cube yellow(1, uiuc::HSLAPixel::YELLOW);
  stacks_[0].push_back(yellow);
}

// Move a Cube from Stack `s1` to Stack `s2`:
void Game::_moveCube(Stack & s1, Stack & s2) {
  Cube cube = s1.removeTop();
  s2.push_back(cube);
}

// Move the cubes in the range [start...end] from `source` to `target`, using spare as a spare spot:
void Game::_move(
  unsigned start, unsigned end,
  Stack & source, Stack & target, Stack & spare,
  unsigned depth
) {
  cout << "Planning (depth=" << depth++ << "): Move [" << start << ".." << end << "] from Stack@" << &source << " -> Stack@" << &target << ", Spare@" << &spare << "]" << endl;

  // Check if we are only moving one cube:
  if (start == end) {
    // If so, move it directly:
    _moveCube( source, target );
    cout << *this << endl;
  } else {
    // Otherwise, use our move strategy:
    _move(start + 1, end  , source, spare , target, depth);
    _move(start    , start, source, target, spare , depth);
    _move(start + 1, end  , spare , target, source, depth);
  }
}

void Game::solve() {
  _move(
    0, stacks_[0].size() - 1,  //< Move the entire set of cubes, [0 .. size-1]
    stacks_[0], //< Source stack is [0]
    stacks_[2], //< Target stack is [2]
    stacks_[1], //< Spare stack is [1]
    0   //< Initial depth (for printouts only) is 0
  );
}

std::ostream& operator<<(std::ostream & os, const Game & game) {
  for (unsigned i = 0; i < game.stacks_.size(); i++) {
    os << "Stack[" << i << ", " << &game.stacks_[i] << "]: " << game.stacks_[i] << endl;
  }
  return os;
}
