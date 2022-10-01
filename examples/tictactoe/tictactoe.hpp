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

#include <MCTS/State.hpp>

#define PLAYER_1_MARKER 'O'
#define PLAYER_2_MARKER 'X'
#define EMPTY_MARKER '.'

/**
 * Tic Tac Toe game action.
 */
struct TicTacToeAction : public GameAction {
    int row;
    int col;
    PlayerMarker playerMarker;
    TicTacToeAction(int row, int col, PlayerMarker playerMarker);
    TicTacToeAction(const TicTacToeAction &other);
    bool operator==(const GameAction &other) const override;
    std::string toString() const override;
};

/**
 * Tic Tac Toe game state.
 */
class TicTacToeGameState : public GameState {
private:
    char board[3][3] {};
    PlayerMarker currentPlayerMarker;
    GameResult gameResult;
    void switchPlayer();
    bool playerWon(PlayerMarker playerMarker) const;
    GameResult calculateGameResult() const;
public:
    explicit TicTacToeGameState(PlayerMarker startingPlayerMarker);
    TicTacToeGameState(const TicTacToeGameState &other);
    PlayerMarker getcurrentPlayerMarker() const override;
    GameResult getGameResult() const override;
    void makeAction(const GameAction *action);
    bool isTerminal() const override;
    std::vector<GameAction *> getLegalActions() const override;
    GameAction *getRandomAction() const;
    GameState *nextState(const GameAction *action) const override;
    double rollout(PlayerMarker maximizingPlayer) const override;
    std::string toString() const override;
};

#endif
