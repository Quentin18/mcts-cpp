# Monte Carlo Tree Search in C++

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=for-the-badge&logoColor=white)](https://opensource.org/licenses/MIT)

This project is an object-oriented implementation of the Monte Carlo Tree Search algorithm in C++.
Check the examples to see how to use this library for your own games.

## Algorithm

[Monte Carlo Tree Search](https://en.wikipedia.org/wiki/Monte_Carlo_tree_search) (MCTS) is an algorithm used for
decision-making in game playing. It combines the exploration of possible moves, done by random simulations, with the
exploitation of the most promising moves, found by evaluating their potential outcomes.

The algorithm builds a tree structure of possible game states, where each node represents a game state, and the edges
are the moves that can be made.

The algorithm iteratively performs four steps: selection, expansion, simulation, and backpropagation. It starts by
selecting the most promising node in the tree, then expand it by adding a new child node representing a new game state.
Then, it simulates random playouts from the new node to estimate the potential outcome of that move. Finally, it
backpropagates the results of the playout to the parent nodes, updating their statistics. This process is repeated
until the algorithm finds the best move to make.

![The four steps of Monte Carlo tree search](https://upload.wikimedia.org/wikipedia/commons/thumb/a/ac/MCTS_steps.png/800px-MCTS_steps.png)

## Getting Started

### Build

This project uses [CMake](https://cmake.org/). To build the repository, run the following commands:
```
git clone https://github.com/Quentin18/mcts-cpp.git
cd mcts-cpp
cmake -S. -Bbuild
cmake --build build
```

### Examples

Then, you can run the examples:

- [Tic-tac-toe](https://en.wikipedia.org/wiki/Tic-tac-toe)
- [Ultimate tic-tac-toe](https://en.wikipedia.org/wiki/Ultimate_tic-tac-toe)

To run the Tic-tac-toe game, use the `tictactoe` program:
```
cd build/bin
./tictactoe
```

To run the UTTT game, use the `uttt` program:
```
cd build/bin
./uttt
```

## Test

The project contains unit tests using [Google Test](https://github.com/google/googletest).
You can run the tests either using `ctest`:
```
cd build
ctest
```
Or directly using `unit_tests`:
```
cd build/bin
./unit_tests
```

## Documentation

You can generate the documentation using `doxygen`:
```
doxygen Doxyfile
```

## Author

[Quentin Deschamps](mailto:quentindeschamps18@gmail.com)
