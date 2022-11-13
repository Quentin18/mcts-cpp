/**
 * Tic Tac Toe game.
 * @author Quentin Deschamps (mailto:quentindeschamps18@gmail.com)
 * @date 2022
 */
#include "tictactoe.hpp"

TicTacToeAction::TicTacToeAction(int row, int col, PlayerMarker playerMarker) :
    row(row), col(col), playerMarker(playerMarker) {}

TicTacToeAction::TicTacToeAction(const TicTacToeAction &other) :
    row(other.row), col(other.col), playerMarker(other.playerMarker) {}

TicTacToeAction &TicTacToeAction::operator=(const TicTacToeAction &other) {
    row = other.row;
    col = other.col;
    playerMarker = other.playerMarker;
    return *this;
}

bool TicTacToeAction::isEmpty() const {
    return playerMarker == EMPTY_MARKER || row == -1 || col == -1;
}

bool TicTacToeAction::operator==(const TicTacToeAction &other) const {
    return row == other.row && col == other.col && playerMarker == other.playerMarker;
}

std::string TicTacToeAction::toString() const {
    std::stringstream ss;
    ss << "row = " << row << ", col = " << col << ", player = " << playerMarker;
    return ss.str();
}

void TicTacToeGameState::resetBoard() {
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            board[row][col] = EMPTY_MARKER;
}

void TicTacToeGameState::switchPlayer() {
    currentPlayerMarker = (currentPlayerMarker == PLAYER_1_MARKER) ? PLAYER_2_MARKER : PLAYER_1_MARKER;
}

bool TicTacToeGameState::playerWon(PlayerMarker playerMarker) const {
    for (int i = 0; i < 3; i++) {
        // Check rows
        if (board[i][0] == playerMarker && board[i][1] == playerMarker && board[i][2] == playerMarker)
            return true;

        // Check columns
        if (board[0][i] == playerMarker && board[1][i] == playerMarker && board[2][i] == playerMarker)
            return true;
    }

    // Check diagonals
    return (
        (board[0][0] == playerMarker && board[1][1] == playerMarker && board[2][2] == playerMarker) ||
        (board[0][2] == playerMarker && board[1][1] == playerMarker && board[2][0] == playerMarker)
    );
}

GameResult TicTacToeGameState::calculateGameResult() const {
    // Check player 1 won
    if (playerWon(PLAYER_1_MARKER))
        return PLAYER_1_WON;

    // Check player 2 won
    if (playerWon(PLAYER_2_MARKER))
        return PLAYER_2_WON;

    // Check not finished
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            if (board[row][col] == EMPTY_MARKER)
                return NOT_FINISHED;

    return DRAW;
}

TicTacToeGameState::TicTacToeGameState(PlayerMarker startingPlayerMarker) :
    currentPlayerMarker(startingPlayerMarker), gameResult(NOT_FINISHED) {
    resetBoard();
}

TicTacToeGameState::TicTacToeGameState(const TicTacToeGameState &other) :
    currentPlayerMarker(other.currentPlayerMarker), gameResult(other.gameResult) {
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            board[row][col] = other.board[row][col];
}

TicTacToeGameState &TicTacToeGameState::operator=(const TicTacToeGameState &other) {
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            board[row][col] = other.board[row][col];

    currentPlayerMarker = other.currentPlayerMarker;
    gameResult = other.gameResult;

    return *this;
}

PlayerMarker TicTacToeGameState::getcurrentPlayerMarker() const {
    return currentPlayerMarker;
}

GameResult TicTacToeGameState::getGameResult() const {
    return gameResult;
}

void TicTacToeGameState::makeAction(const TicTacToeAction &action) {
    // Check that the action is legal action
    if (board[action.row][action.col] != EMPTY_MARKER)
        throw std::runtime_error("Illegal action: " + action.toString());

    // Make action
    board[action.row][action.col] = action.playerMarker;
}

bool TicTacToeGameState::isTerminal() const {
    return gameResult != NOT_FINISHED;
}

std::vector<TicTacToeAction> TicTacToeGameState::getLegalActions() const {
    std::vector<TicTacToeAction> actions;

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (board[row][col] == EMPTY_MARKER) {
                TicTacToeAction action(row, col, currentPlayerMarker);
                actions.push_back(action);
            }
        }
    }

    return actions;
}

TicTacToeAction TicTacToeGameState::getRandomAction() const {
    uint actionIndex;

    // Get legal actions
    auto legalActions = getLegalActions();

    // Check if there are legal actions
    if (legalActions.empty())
        throw std::runtime_error("No legal actions.");

    // Select random action
    actionIndex = rand() % legalActions.size();

    return legalActions[actionIndex];
}

TicTacToeGameState TicTacToeGameState::nextState(const TicTacToeAction &action) const {
    // Create new state from current
    TicTacToeGameState newState(*this);

    // Make action
    newState.makeAction(action);

    // Calculate game result
    newState.gameResult = newState.calculateGameResult();

    // Change the player turn
    newState.switchPlayer();

    return newState;
}

double TicTacToeGameState::rollout(PlayerMarker maximizingPlayer) const {
    GameResult result;

    if (isTerminal()) {
        result = getGameResult();
    } else {
        // Init simulation to current state
        TicTacToeGameState currentState(*this);
        TicTacToeAction action;

        // Run game until we reach a terminal state
        do {
            action = currentState.getRandomAction();
            currentState.makeAction(action);
            currentState.gameResult = currentState.calculateGameResult();
            currentState.switchPlayer();
        } while (!currentState.isTerminal());

        result = currentState.getGameResult();
    }

    // Evaluate game to maximize player
    if ((result == PLAYER_1_WON && maximizingPlayer == PLAYER_1_MARKER) ||
        (result == PLAYER_2_WON && maximizingPlayer == PLAYER_2_MARKER)) {
        return 1.0;
    }
    if (result == DRAW) {
        return 0.5;
    }
    return 0.0;
}

std::string TicTacToeGameState::toString() const {
    std::stringstream ss;

    ss << "+---+---+---+" << std::endl;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++)
            ss << "| " << board[row][col] << " ";
        ss << "|" << std::endl << "+---+---+---+" << std::endl;
    }

    return ss.str();
}
