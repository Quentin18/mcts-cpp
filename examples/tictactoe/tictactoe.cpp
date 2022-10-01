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

bool TicTacToeAction::operator==(const GameAction &other) const {
    // Cast action
    const auto &o = (const TicTacToeAction &) other;
    return row == o.row && col == o.col && playerMarker == o.playerMarker;
}

std::string TicTacToeAction::toString() const {
    std::stringstream ss;
    ss << "row = " << row << ", col = " << col << ", player = " << playerMarker;
    return ss.str();
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
    currentPlayerMarker(startingPlayerMarker),
    gameResult(NOT_FINISHED) {
    // Initialize board
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            board[row][col] = EMPTY_MARKER;
}

TicTacToeGameState::TicTacToeGameState(const TicTacToeGameState &other) :
    currentPlayerMarker(other.currentPlayerMarker),
    gameResult(other.gameResult) {
    // Copy board
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            board[row][col] = other.board[row][col];
}

PlayerMarker TicTacToeGameState::getcurrentPlayerMarker() const {
    return currentPlayerMarker;
}

GameResult TicTacToeGameState::getGameResult() const {
    return gameResult;
}

void TicTacToeGameState::makeAction(const GameAction *action) {
    // Cast action
    auto a = (TicTacToeAction *) action;

    // Check that the action is legal action
    if (board[a->row][a->col] != EMPTY_MARKER)
        throw std::runtime_error("Illegal action: " + a->toString());

    // Make action
    board[a->row][a->col] = a->playerMarker;
}

bool TicTacToeGameState::isTerminal() const {
    return gameResult != NOT_FINISHED;
}

std::vector<GameAction *> TicTacToeGameState::getLegalActions() const {
    std::vector<GameAction *> actions;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (board[row][col] == EMPTY_MARKER)
                actions.push_back(new TicTacToeAction(row, col, currentPlayerMarker));
        }
    }
    return actions;
}

GameAction *TicTacToeGameState::getRandomAction() const {
    GameAction *action;
    uint actionIndex;

    // Get legal actions
    auto legalActions = getLegalActions();

    // Check if there are legal actions
    if (legalActions.empty())
        throw std::runtime_error("No legal actions.");

    // Select random action
    actionIndex = rand() % legalActions.size();
    for (uint i = 0; i < legalActions.size(); ++i) {
        if (i == actionIndex)
            action = legalActions[i];
        else
            delete legalActions[i];
    }

    return action;
}

GameState *TicTacToeGameState::nextState(const GameAction *action) const {
    // Create new state from current
    auto newState = new TicTacToeGameState(*this);

    // Make action
    newState->makeAction(action);

    // Calculate game result
    newState->gameResult = newState->calculateGameResult();

    // Change the player turn
    newState->switchPlayer();

    return newState;
}

double TicTacToeGameState::rollout(PlayerMarker maximizingPlayer) const {
    GameResult result;

    if (isTerminal()) {
        result = getGameResult();
    } else {
        // Init simulation to current state
        auto currentState = new TicTacToeGameState(*this);
        GameAction *action;

        // Run game until we reach a terminal state
        do {
            action = currentState->getRandomAction();
            currentState->makeAction(action);
            currentState->gameResult = currentState->calculateGameResult();
            currentState->switchPlayer();
            delete action;
        } while (!currentState->isTerminal());

        result = currentState->getGameResult();
        delete currentState;
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
