/**
 * Manage game state.
 * @author Quentin Deschamps (mailto:quentindeschamps18@gmail.com)
 * @date 2022
 */
#ifndef STATE_HPP
#define STATE_HPP

#include <string>
#include <vector>

/**
 * Player marker type.
 */
typedef char PlayerMarker;

/**
 * Game results enum.
 */
enum GameResult {
    PLAYER_1_WON, PLAYER_2_WON, DRAW, NOT_FINISHED
};

/**
 * Base class for game state.
 * This class needs to be implemented for each game.
 */
template<class Action, class State>
class GameState {
public:
    /**
     * Destroy a game state.
     */
    virtual ~GameState() = default;

    /**
     * Get the current player marker.
     * @return player marker.
     */
    virtual PlayerMarker getcurrentPlayerMarker() const = 0;

    /**
     * Get legal actions from current state.
     * @return legal actions.
     */
    virtual std::vector<Action> getLegalActions() const = 0;

    /**
     * Get the next state from the current state after playing an action.
     * @param action action to play.
     * @return next state.
     */
    virtual State nextState(const Action &action) const = 0;

    /**
     * Simulate a game from the current game state and return a score between 0 and 1.
     * @param maximizingPlayer player to maximize.
     * @return score.
     */
    virtual double rollout(PlayerMarker maximizingPlayer) const = 0;

    /**
     * Return true if the state is terminal, false otherwise.
     * @return terminal.
     */
    virtual bool isTerminal() const = 0;

    /**
     * Get the game result of the current state.
     * @return game result.
     */
    virtual GameResult getGameResult() const = 0;

    /**
     * Represents a state as a string.
     * @return state as string.
     */
    virtual std::string toString() const { return "GameState::toString not implemented"; };
};

#endif
