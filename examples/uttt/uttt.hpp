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

#include <MCTS/Action.hpp>
#include <MCTS/State.hpp>

#define PLAYER_1_MARKER 'O'
#define PLAYER_2_MARKER 'X'
#define EMPTY_MARKER '.'
#define DRAW_MARKER '~'

/**
 * Ultimate Tic Tac Toe game action.
 */
struct UltimateTicTacToeAction : public GameAction<UltimateTicTacToeAction> {
    int row = -1;
    int col = -1;
    PlayerMarker playerMarker = EMPTY_MARKER;
    UltimateTicTacToeAction() = default;
    UltimateTicTacToeAction(int row, int col, PlayerMarker playerMarker);
    UltimateTicTacToeAction(const UltimateTicTacToeAction &other);
    UltimateTicTacToeAction &operator=(const UltimateTicTacToeAction &other);
    bool isEmpty() const override;
    bool operator==(const UltimateTicTacToeAction &other) const override;
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
    bool isLegalAction(const UltimateTicTacToeAction &action) const;
    void makeAction(const UltimateTicTacToeAction &action);
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
class UltimateTicTacToeGameState : public GameState<UltimateTicTacToeAction, UltimateTicTacToeGameState> {
private:
    TicTacToeGrid masterGrid;
    TicTacToeGrid smallGrids[3][3];
    PlayerMarker currentPlayerMarker;
    UltimateTicTacToeAction lastAction;
    GameResult gameResult;
    void switchPlayer();
    bool playerWon(PlayerMarker playerMarker) const;
    GameResult calculateGameResult() const;
public:
    explicit UltimateTicTacToeGameState(PlayerMarker startingPlayerMarker);
    UltimateTicTacToeGameState(const UltimateTicTacToeGameState &other);
    UltimateTicTacToeGameState &operator=(const UltimateTicTacToeGameState &other);
    PlayerMarker getcurrentPlayerMarker() const override;
    GameResult getGameResult() const override;
    bool isLegalAction(const UltimateTicTacToeAction &action) const;
    void makeAction(const UltimateTicTacToeAction &action);
    bool isTerminal() const override;
    std::vector<UltimateTicTacToeAction> getLegalActions() const override;
    UltimateTicTacToeAction getRandomAction() const;
    UltimateTicTacToeGameState nextState(const UltimateTicTacToeAction &action) const override;
    double rollout(PlayerMarker maximizingPlayer) const override;
    std::string toString() const override;
};

#endif
