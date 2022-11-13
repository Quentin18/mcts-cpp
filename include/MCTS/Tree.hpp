/**
 * Manage game tree.
 * @author Quentin Deschamps (mailto:quentindeschamps18@gmail.com)
 * @date 2022
 */
#ifndef TREE_HPP
#define TREE_HPP

#include <chrono>
#include <iostream>

#include <MCTS/Node.hpp>
#include <MCTS/State.hpp>

/**
 * Manage game tree.
 */
class GameTree {
private:
    GameNode *root;
    GameNode *currentRoot;
public:
    /**
     * Create a game tree.
     * @param startingState starting state.
     * @param maximizingPlayer player to maximize.
     */
    explicit GameTree(GameState *startingState, PlayerMarker maximizingPlayer);

    /**
     * Destroy a game tree.
     */
    ~GameTree();

    /**
     * Advance the tree to the next node.
     * @param action last action.
     */
    void advanceTree(const GameAction *action);

    /**
     * Get the current game state.
     * @return game state.
     */
    GameState *getCurrentState() const;

    /**
     * Select a node to expand according to UCT policy.
     * @param c exploration parameter.
     * @return selected node.
     */
    GameNode *select(double c = 1.41);

    /**
     * Select the best child.
     * @return selected node.
     */
    GameNode *selectBestChild();

    /**
     * Grow the tree with rollouts.
     * @param maxIter maximum iteration.
     * @param maxSeconds maximum seconds.
     * @param debug true to display logs.
     */
    void growTree(uint maxIter, uint maxSeconds, bool debug);

    /**
     * Print the tree stats.
     */
    void printStats() const;
};

#endif
