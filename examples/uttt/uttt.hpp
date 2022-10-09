/**
 * Ultimate Tic Tac Toe game.
 * @author Quentin Deschamps (mailto:quentindeschamps18@gmail.com)
 * @date 2022
 */
#ifndef UTTT_HPP
#define UTTT_HPP

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <MCTS/State.hpp>

#define PLAYER_1_MARKER 'O'
#define PLAYER_2_MARKER 'X'
#define EMPTY_MARKER '.'
#define DRAW_MARKER '~'

/**
 * Ultimate Tic Tac Toe game action.
 */
struct UltimateTicTacToeAction : public GameAction {
    int row;
    int col;
    PlayerMarker playerMarker;
    UltimateTicTacToeAction(int row, int col, PlayerMarker playerMarker);
    UltimateTicTacToeAction(const UltimateTicTacToeAction &other);
    bool operator==(const GameAction &other) const override;
    std::string toString() const override;
};

/**
 * Manage Tic Tac Toe grid.
 */
class TicTacToeGrid {
private:
    char grid[3][3] {};
    GameResult gameResult;
    GameResult calculateGameResult() const;
public:
    TicTacToeGrid();
    TicTacToeGrid(const TicTacToeGrid &other);
    char get(int row, int col) const;
    bool isEmpty(int row, int col) const;
    bool isLegalAction(const UltimateTicTacToeAction *action) const;
    void makeAction(const GameAction *action);
    void updateGameResult();
    bool playerWon(PlayerMarker playerMarker) const;
    bool isFull() const;
    GameResult getGameResult() const;
    std::string toString() const;
    TicTacToeGrid& operator=(const TicTacToeGrid &other);
};

/**
 * Ultimate Tic Tac Toe game state.
 */
class UltimateTicTacToeGameState : public GameState {
private:
    TicTacToeGrid masterGrid;
    TicTacToeGrid smallGrids[3][3];
    PlayerMarker currentPlayerMarker;
    UltimateTicTacToeAction *lastAction;
    GameResult gameResult;
    void switchPlayer();
    bool playerWon(PlayerMarker playerMarker) const;
    GameResult calculateGameResult() const;
public:
    explicit UltimateTicTacToeGameState(PlayerMarker startingPlayerMarker);
    UltimateTicTacToeGameState(const UltimateTicTacToeGameState &other);
    ~UltimateTicTacToeGameState();
    PlayerMarker getcurrentPlayerMarker() const override;
    GameResult getGameResult() const override;
    bool isLegalAction(const UltimateTicTacToeAction *action) const;
    void makeAction(const GameAction *action);
    bool isTerminal() const override;
    std::vector<GameAction *> getLegalActions() const override;
    GameAction *getRandomAction() const;
    GameState *nextState(const GameAction *action) const override;
    double rollout(PlayerMarker maximizingPlayer) const override;
    std::string toString() const override;
};

#endif
