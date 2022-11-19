/**
 * Manage game agents.
 * @author Quentin Deschamps (mailto:quentindeschamps18@gmail.com)
 * @date 2022
 */
#ifndef AGENT_HPP
#define AGENT_HPP

#include <iostream>
#include <stdexcept>
#include <vector>

#include <MCTS/Tree.hpp>

/**
 * Base class for game agent.
 */
template<class Action, class State>
class Agent {
public:
    /**
     * Get an action from the current state.
     * @param state current game state.
     * @param lastAction last action played.
     * @return next action to play.
     */
    virtual Action getAction(const State &state, const Action &lastAction) = 0;
};

/**
 * Monte Carlo Tree Search agent.
 * @cite https://en.wikipedia.org/wiki/Monte_Carlo_tree_search
 */
template<class Action, class State>
class MonteCarloTreeSearchAgent : public Agent<Action, State> {
private:
    GameTree<Action, State> *tree;
    uint maxIter;
    uint maxSeconds;
    bool debug;
public:
    /**
     * Create a Monte Carlo Tree Search agent.
     * @param startingState starting game state.
     * @param player player marker.
     * @param maxIter maximum iteration per round.
     * @param maxSeconds maximum seconds per round.
     * @param debug true to display logs.
     */
    MonteCarloTreeSearchAgent(
            const State &startingState,
            PlayerMarker player,
            uint maxIter = 100000,
            uint maxSeconds = 5,
            bool debug = true
    ) : maxIter(maxIter), maxSeconds(maxSeconds), debug(debug) {
        tree = new GameTree<Action, State>(startingState, player);
    }

    /**
     * Destroy a MCTS agent.
     */
    ~MonteCarloTreeSearchAgent() {
        delete tree;
    }

    /**
     * Get an action from the current state using MCTS.
     * @param state current game state.
     * @param lastAction last action played.
     * @return next action to play.
     */
    Action getAction(const State &state, const Action &lastAction) override {
        // Print legal actions
        if (debug) {
            std::cout << "Legal actions:" << std::endl;
            for (const auto &action : state.getLegalActions())
                std::cout << action.toString() << std::endl;
        }

        // Advance the tree with last action
        if (!lastAction.isEmpty())
            tree->advanceTree(lastAction);

        // If we have a terminal state, we cannot do anything
        if (tree->getCurrentState().isTerminal())
            throw std::invalid_argument("The current state is terminal.");

        // Grow the tree
        tree->growTree(maxIter, maxSeconds, debug);

        // Print stats
        if (debug)
            tree->printStats();

        // Select the best child
        GameNode<Action, State> *bestChild = tree->selectBestChild();
        if (bestChild == nullptr)
            throw std::invalid_argument("Tree root has no children.");

        // Get corresponding action
        Action bestAction = bestChild->getLastAction();
        std::cout << "MCTS action selected: " << bestAction.toString() << std::endl;

        // Advance tree with the chosen action
        tree->advanceTree(bestAction);

        return bestAction;
    }
};

/**
 * Random agent.
 */
template<class Action, class State>
class RandomAgent : public Agent<Action, State> {
public:
    /**
     * Get a random action from the current state.
     * @param state current game state.
     * @param lastAction last action played.
     * @return next action to play.
     */
    Action getAction(const State &state, const Action &lastAction) override {
        uint actionIndex;

        // Get legal actions
        auto legalActions = state.getLegalActions();

        // Check if there are legal actions
        if (legalActions.empty())
            throw std::runtime_error("No legal actions.");

        // Select random action
        actionIndex = rand() % legalActions.size();
        std::cout << "Random action selected: " << legalActions[actionIndex].toString() << std::endl;

        return legalActions[actionIndex];
    }
};

#endif
