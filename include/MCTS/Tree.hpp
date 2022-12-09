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

/**
 * Manage game tree.
 */
template<class Action, class State>
class GameTree {
private:
    GameNode<Action, State> *root;
    GameNode<Action, State> *currentRoot;
public:
    /**
     * Create a game tree.
     * @param startingState starting state.
     * @param maximizingPlayer player to maximize.
     */
    GameTree(const State &startingState, PlayerMarker maximizingPlayer) {
        root = new GameNode<Action, State>(startingState, maximizingPlayer);
        currentRoot = root;
    }

    /**
     * Destroy a game tree.
     */
    ~GameTree() {
        delete root;
    }

    /**
     * Advance the tree to the next node.
     * @param action last action.
     */
    void advanceTree(const Action &action) {
        currentRoot = currentRoot->advanceTree(action);
    }

    /**
     * Get the current game state.
     * @return game state.
     */
    const State &getCurrentState() const {
        return currentRoot->getCurrentState();
    }

    /**
     * Select a node to expand according to UCT policy.
     * @param c exploration parameter.
     * @return selected node.
     */
    GameNode<Action, State> *select(double c = 1.41) {
        GameNode<Action, State> *node = currentRoot;

        while (!node->isTerminal()) {
            if (!node->isFullyExpanded())
                return node;
            else
                node = node->selectBestChild(c);
        }

        return node;
    }

    /**
     * Select the best child.
     * @return selected node.
     */
    GameNode<Action, State> *selectBestChild() {
        return currentRoot->selectBestChild(0.0);
    }

    /**
     * Grow the tree with rollouts.
     * @param maxIter maximum iteration.
     * @param maxSeconds maximum seconds.
     * @param debug true to display logs.
     */
    void growTree(uint maxIter, uint maxSeconds, bool debug) {
        std::chrono::duration<double> elapsedSeconds{};
        GameNode<Action, State> *node;
        uint iter = 0;

        // Start timer
        std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

        // Grow tree
        do {
            // Select node to expand according to tree policy
            node = select();

            // Expand it: this will perform a rollout and back propagate the results
            node->expand();

            // Check if we need to stop
            ++iter;
            elapsedSeconds = std::chrono::system_clock::now() - start;
        } while (iter < maxIter && elapsedSeconds.count() < maxSeconds);

        if (debug)
            std::cout << "Made " << iter << " iterations in " << elapsedSeconds.count() << " seconds." << std::endl;
    }

    /**
     * Print the tree stats.
     */
    void printStats() const {
        currentRoot->printStats();
    }
};

#endif
