/**
 * Tic Tac Toe game.
 * @author Quentin Deschamps (mailto:quentindeschamps18@gmail.com)
 * @date 2022
 */
#ifndef TICTACTOE_HPP
#define TICTACTOE_HPP

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <MCTS/Action.hpp>
#include <MCTS/State.hpp>
#include <MCTS/Utils.hpp>

#define PLAYER_1_MARKER 'O'
#define PLAYER_2_MARKER 'X'
#define EMPTY_MARKER '.'

/**
 * Tic Tac Toe game action.
 */
struct TicTacToeAction : public GameAction<TicTacToeAction> {
    int row = -1;
    int col = -1;
    PlayerMarker playerMarker = EMPTY_MARKER;
    TicTacToeAction() = default;
    TicTacToeAction(int row, int col, PlayerMarker playerMarker);
    TicTacToeAction(const TicTacToeAction &other);
    TicTacToeAction &operator=(const TicTacToeAction &other);
    bool isEmpty() const override;
    bool operator==(const TicTacToeAction &other) const override;
    std::string toString() const override;
};

/**
 * Tic Tac Toe game state.
 */
class TicTacToeGameState : public GameState<TicTacToeAction, TicTacToeGameState> {
private:
    char board[3][3] {};
    PlayerMarker currentPlayerMarker;
    GameResult gameResult;
    void resetBoard();
    void switchPlayer();
    bool playerWon(PlayerMarker playerMarker) const;
    GameResult calculateGameResult() const;
public:
    explicit TicTacToeGameState(PlayerMarker startingPlayerMarker);
    TicTacToeGameState(const TicTacToeGameState &other);
    TicTacToeGameState &operator=(const TicTacToeGameState &other);
    PlayerMarker getcurrentPlayerMarker() const override;
    GameResult getGameResult() const override;
    void makeAction(const TicTacToeAction &action);
    bool isTerminal() const override;
    std::vector<TicTacToeAction> getLegalActions() const override;
    TicTacToeAction getRandomAction() const;
    TicTacToeGameState nextState(const TicTacToeAction &action) const override;
    double rollout(PlayerMarker maximizingPlayer) const override;
    std::string toString() const override;
};

#endif
