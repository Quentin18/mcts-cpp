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
template<class Action, class State>
class GameNode {
private:
    GameNode *parent;
    std::vector<GameNode *> children;
    State currentState;
    Action lastAction;
    std::vector<Action> untriedActions;
    bool terminal;
    uint visits;
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
    GameNode(GameNode *parent, const State &currentState, const Action &lastAction, PlayerMarker maximizingPlayer) :
            parent(parent),
            currentState(currentState),
            lastAction(lastAction),
            visits(0),
            score(0.0),
            maximizingPlayer(maximizingPlayer) {
        untriedActions = currentState.getLegalActions();
        terminal = currentState.isTerminal();
    }

    /**
     * Create a game node with no parent node.
     * @param currentState current game state.
     * @param maximizingPlayer player to maximize.
     */
    GameNode(const State &currentState, PlayerMarker maximizingPlayer) :
            parent(nullptr),
            currentState(currentState),
            visits(0),
            score(0.0),
            maximizingPlayer(maximizingPlayer) {
        untriedActions = currentState.getLegalActions();
        terminal = currentState.isTerminal();
    }

    /**
     * Destroy a game node.
     */
    ~GameNode() {
        for (auto *child : children)
            delete child;
    }

    /**
     * Advance the tree to the next node.
     * @param action last action.
     */
    GameNode *advanceTree(const Action &action) {
        GameNode<Action, State> *nextNode;

        // Find the child corresponding to the action
        auto it = std::find_if(
                children.begin(),
                children.end(),
                [action](const GameNode *child) {
                    return child->getLastAction() == action;
                }
        );

        // Check if we found the child, otherwise we create a new node
        if (it == children.end()) {
            std::cerr << "INFO: Child not found. Had to start over." << std::endl;
            nextNode = new GameNode(currentState.nextState(action), maximizingPlayer);  // TODO free memory
        } else {
            nextNode = *it;
        }

        // Reset parent to set the new node as root
        nextNode->parent = nullptr;

        return nextNode;
    }

    /**
     * Calculate the win rate of a player.
     * @param player player marker.
     * @return win rate.
     */
    double calculateWinRate(PlayerMarker player) const {
        return player == maximizingPlayer ? score / visits : 1.0 - score / visits;
    }

    /**
     * Calculate the UCT policy.
     * @param child child node to apply policy.
     * @param player player marker.
     * @param c exploration parameter.
     * @return UCT policy.
     */
    double uct(const GameNode *child, PlayerMarker player, double c) const {
        return child->calculateWinRate(player) + c * sqrt(log((double) visits) / (double) child->visits);
    }

    /**
     * Select a node to expand according to UCT policy.
     * @param c exploration parameter.
     * @return selected node.
     */
    GameNode *selectBestChild(double c) const {
        // No children
        if (children.empty())
            throw std::runtime_error("No child to select.");

        // One child
        if (children.size() == 1)
            return children.at(0);

        // Return the best child according to the UCT policy
        PlayerMarker player = currentState.getcurrentPlayerMarker();
        return *std::max_element(
                children.begin(),
                children.end(),
                [this, c, player](const GameNode *firstChild, const GameNode *secondChild) {
                    return this->uct(firstChild, player, c) < this->uct(secondChild, player, c);
                }
        );
    }

    /**
     * Get current game state.
     * @return game state.
     */
    const State &getCurrentState() const {
        return currentState;
    }

    /**
     * Get last action.
     * @return action.
     */
    const Action &getLastAction() const {
        return lastAction;
    }

    /**
     * Return true if the node is terminal, false otherwise.
     * @return terminal.
     */
    bool isTerminal() const {
        return terminal;
    }

    /**
     * Return true if the node is fully expanded, false otherwise.
     * @return fully expanded.
     */
    bool isFullyExpanded() const {
        return isTerminal() || untriedActions.empty();
    }

    /**
     * Expand tht node.
     */
    void expand() {
        if (isTerminal()) {
            rollout();
            return;
        } else if (isFullyExpanded()) {
            throw std::runtime_error("Cannot expand node.");
        }

        // Get next untried action
        Action nextAction = untriedActions.front();
        untriedActions.erase(untriedActions.begin());

        // Get next state
        State nextState = currentState.nextState(nextAction);

        // Create new node and add it to the tree
        auto *newNode = new GameNode(this, nextState, nextAction, maximizingPlayer);
        children.push_back(newNode);

        // Rollout and update stats
        newNode->rollout();
    }

    /**
     * Simulate a game from the current state and back propagate the results.
     */
    void rollout() {
        backPropagate(currentState.rollout(maximizingPlayer), 1);
    }

    /**
     * Back propagate the results of simulation.
     * @param w number of wins.
     * @param n number of simulations.
     */
    void backPropagate(double w, int n) {
        score += w;
        visits += n;
        if (parent != nullptr)
            parent->backPropagate(w, n);
    }

    /**
     * Print the node stats.
     */
    void printStats() {
        std::cout << "INFOS:" << std::endl;
        if (visits == 0) {
            std::cout << "Tree not expanded yet" << std::endl;
            return;
        }
        std::cout << "Visits: " << visits << std::endl
                  << "Branching factor at root: " << children.size() << std::endl
                  << "Chances of player winning: " << std::setprecision(PRECISION)
                  << 100.0 * (score / visits) << "%" << std::endl;

        // Sort children based on win rate of player's turn for this node
        PlayerMarker player = currentState.getcurrentPlayerMarker();
        std::sort(
                children.begin(),
                children.end(),
                [player](const GameNode *firstChild, const GameNode *secondChild) {
                    return firstChild->calculateWinRate(player) > secondChild->calculateWinRate(player);
                });

        // Print best actions
        std::cout << "Best actions:" << std::endl;
        for (std::size_t i = 0; i < children.size(); ++i) {
            std::cout << i + 1 << ". "
                      << children.at(i)->lastAction.toString()
                      << " -> " << std::setprecision(PRECISION)
                      << 100.0 * children.at(i)->calculateWinRate(player)
                      << "%" << std::endl;
        }
    }
};

#endif
