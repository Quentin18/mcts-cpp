/**
 * Manage game node.
 * @author Quentin Deschamps (mailto:quentindeschamps18@gmail.com)
 * @date 2022
 */
#ifndef NODE_HPP
#define NODE_HPP

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <MCTS/State.hpp>

#define PRECISION 4

/**
 * Manage game node.
 */
class GameNode {
private:
    GameNode *parent;
    std::vector<GameNode *> children;
    GameState *currentState;
    GameAction *lastAction;
    std::vector<GameAction *> untriedActions;
    bool terminal;
    uint numberOfSimulations;
    double score;
    PlayerMarker maximizingPlayer;
public:
    /**
     * Create a game node.
     * @param parent parent node.
     * @param currentState current game state.
     * @param lastAction last action.
     * @param maximizingPlayer player to maximize.
     */
    GameNode(GameNode *parent, GameState *currentState, GameAction *lastAction, PlayerMarker maximizingPlayer);

    /**
     * Create a game node with no parent node.
     * @param currentState current game state.
     * @param maximizingPlayer player to maximize.
     */
    explicit GameNode(GameState *currentState, PlayerMarker maximizingPlayer);

    /**
     * Destroy a game node.
     */
    ~GameNode();

    /**
     * Advance the tree to the next node.
     * @param action last action.
     */
    GameNode *advanceTree(const GameAction *action);

    /**
     * Calculate the win rate of a player.
     * @param player player marker.
     * @return win rate.
     */
    double calculateWinRate(PlayerMarker player) const;

    /**
     * Calculate the UCT policy.
     * @param child child node to apply policy.
     * @param player player marker.
     * @param c exploration parameter.
     * @return UCT policy.
     */
    double uct(const GameNode *child, PlayerMarker player, double c) const;

    /**
     * Select a node to expand according to UCT policy.
     * @param c exploration parameter.
     * @return selected node.
     */
    GameNode *selectBestChild(double c) const;

    /**
     * Get current game state.
     * @return game state.
     */
    GameState *getCurrentState() const;

    /**
     * Get last action.
     * @return action.
     */
    GameAction *getLastAction() const;

    /**
     * Return true if the node is terminal, false otherwise.
     * @return terminal.
     */
    bool isTerminal() const;

    /**
     * Return true if the node is fully expanded, false otherwise.
     * @return fully expanded.
     */
    bool isFullyExpanded() const;

    /**
     * Expand tht node.
     */
    void expand();

    /**
     * Simulate a game from the current state and back propagate the results.
     */
    void rollout();

    /**
     * Back propagate the results of simulation.
     * @param w number of wins.
     * @param n number of simulations.
     */
    void backPropagate(double w, int n);

    /**
     * Print the node stats.
     */
    void printStats();
};

#endif
