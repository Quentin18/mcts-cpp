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

#include <MCTS/State.hpp>
#include <MCTS/Tree.hpp>

/**
 * Base class for game agent.
 */
class Agent {
public:
    /**
     * Get an action from the current state.
     * @param state current game state.
     * @param lastAction last action played.
     * @return next action to play.
     */
    virtual GameAction *getAction(const GameState *state, const GameAction *lastAction) = 0;
};

/**
 * Monte Carlo Tree Search agent.
 * @cite https://en.wikipedia.org/wiki/Monte_Carlo_tree_search
 */
class MonteCarloTreeSearchAgent : public Agent {
private:
    GameTree *tree;
    PlayerMarker player;
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
    explicit MonteCarloTreeSearchAgent(
            GameState *startingState,
            PlayerMarker player,
            uint maxIter = 100000,
            uint maxSeconds = 10,
            bool debug = true
    );

    /**
     * Destroy a MCTS agent.
     */
    ~MonteCarloTreeSearchAgent();

    /**
     * Get an action from the current state using MCTS.
     * @param state current game state.
     * @param lastAction last action played.
     * @return next action to play.
     */
    GameAction *getAction(const GameState *state, const GameAction *lastAction) override;
};

/**
 * Random agent.
 */
class RandomAgent : public Agent {
public:
    /**
     * Get an random action from the current state.
     * @param state current game state.
     * @param lastAction last action played.
     * @return next action to play.
     */
    GameAction *getAction(const GameState *state, const GameAction *lastAction) override;
};

#endif
