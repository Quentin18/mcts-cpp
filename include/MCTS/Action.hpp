/**
 * Manage game action.
 * @author Quentin Deschamps (mailto:quentindeschamps18@gmail.com)
 * @date 2022
 */
#ifndef ACTION_HPP
#define ACTION_HPP

#include <string>

/**
 * Base class for game action.
 * This class needs to be implemented for each game.
 */
template<class Action>
struct GameAction {
    /**
     * Destroy a game action.
     */
    virtual ~GameAction() = default;

    /**
     * Operator == to compare game actions.
     * @param other other action.
     * @return true if the actions are the same, false otherwise.
     */
    virtual bool operator==(const Action &other) const = 0;

    /**
     * Return true if the action is empty, false otherwise.
     * @return empty.
     */
    virtual bool isEmpty() const = 0;

    /**
     * Represents an action as a string.
     * @return action as string.
     */
    virtual std::string toString() const { return "GameAction::toString not implemented"; };
};

#endif
