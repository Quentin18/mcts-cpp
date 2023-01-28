/**
 * Connect 4 game.
 * @author Quentin Deschamps (mailto:quentindeschamps18@gmail.com)
 * @date 2022
 */
#ifndef CONNECT4_HPP
#define CONNECT4_HPP

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include <MCTS/Action.hpp>
#include <MCTS/State.hpp>
#include <MCTS/Utils.hpp>

#define PLAYER_1_MARKER 'O'
#define PLAYER_2_MARKER 'X'
#define EMPTY_MARKER '.'
#define HEIGHT 6
#define WIDTH 7

/**
 * Connect 4 game action.
 */
struct ConnectFourAction : public GameAction<ConnectFourAction> {
    int col = -1;
    PlayerMarker playerMarker = EMPTY_MARKER;

    ConnectFourAction() = default;

    ConnectFourAction(int col, PlayerMarker playerMarker);

    ConnectFourAction(const ConnectFourAction &other);

    ConnectFourAction &operator=(const ConnectFourAction &other);

    bool isEmpty() const override;

    bool operator==(const ConnectFourAction &other) const override;

    std::string toString() const override;
};

/**
 * Connect 4 game state.
 */
class ConnectFourGameState : public GameState<ConnectFourAction, ConnectFourGameState> {
private:
    char board[HEIGHT][WIDTH]{};
    std::unordered_map<int, int> colHeight;
    PlayerMarker currentPlayerMarker;
    ConnectFourAction lastAction;
    GameResult gameResult;

    int getColHeight(int col) const;

    void resetBoard();

    void switchPlayer();

    bool checkDraw() const;

    GameResult calculateGameResult() const;

public:
    explicit ConnectFourGameState(PlayerMarker startingPlayerMarker);

    ConnectFourGameState(const ConnectFourGameState &other);

    ConnectFourGameState &operator=(const ConnectFourGameState &other);

    PlayerMarker getcurrentPlayerMarker() const override;

    GameResult getGameResult() const override;

    void makeAction(const ConnectFourAction &action);

    bool isTerminal() const override;

    std::vector<ConnectFourAction> getLegalActions() const override;

    ConnectFourAction getRandomAction() const;

    ConnectFourGameState nextState(const ConnectFourAction &action) const override;

    double rollout(PlayerMarker maximizingPlayer) const override;

    std::string toString() const override;
};

#endif
