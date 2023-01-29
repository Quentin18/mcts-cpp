/**
 * Connect 4 game.
 * @author Quentin Deschamps (mailto:quentindeschamps18@gmail.com)
 * @date 2022
 */
#include "connect4.hpp"

ConnectFourAction::ConnectFourAction(int col, PlayerMarker playerMarker) :
        col(col), playerMarker(playerMarker) {}

ConnectFourAction::ConnectFourAction(const ConnectFourAction &other) :
        col(other.col), playerMarker(other.playerMarker) {}

ConnectFourAction &ConnectFourAction::operator=(const ConnectFourAction &other) {
    col = other.col;
    playerMarker = other.playerMarker;
    return *this;
}

bool ConnectFourAction::isEmpty() const {
    return playerMarker == EMPTY_MARKER || col == -1;
}

bool ConnectFourAction::operator==(const ConnectFourAction &other) const {
    return col == other.col && playerMarker == other.playerMarker;
}

std::string ConnectFourAction::toString() const {
    std::stringstream ss;
    ss << "col = " << col << ", player = " << playerMarker;
    return ss.str();
}

int ConnectFourGameState::getColHeight(int col) const {
    return colHeight.at(col);
}

void ConnectFourGameState::resetBoard() {
    for (int col = 0; col < WIDTH; ++col) {
        for (int row = 0; row < HEIGHT; ++row)
            board[row][col] = EMPTY_MARKER;
        colHeight[col] = 0;
    }
}

void ConnectFourGameState::switchPlayer() {
    currentPlayerMarker = currentPlayerMarker == PLAYER_1_MARKER ? PLAYER_2_MARKER : PLAYER_1_MARKER;
}

bool ConnectFourGameState::checkDraw() const {
    for (int col = 0; col < WIDTH; ++col)
        if (getColHeight(col) < HEIGHT)
            return false;

    return true;
}

GameResult ConnectFourGameState::calculateGameResult() const {
    if (lastAction.isEmpty())
        return NOT_FINISHED;

    if (checkDraw())
        return DRAW;

    // Get row and column of last move
    int col = lastAction.col;
    int row = getColHeight(col) - 1;
    char player = board[row][col];

    // Counter for 4 in a row
    uint counter = 1;

    // Iterators that holds current position to check
    int rowIter = row, colIter = col - 1;

    // First check row by expanding left from piece placed
    while (colIter >= 0 && board[rowIter][colIter] == player) {
        ++counter;
        --colIter;
    }

    // Next go right from the piece placed
    colIter = col + 1;

    while (colIter < WIDTH && board[rowIter][colIter] == player) {
        ++counter;
        ++colIter;
    }

    // Check if there were 4 consecutive pieces
    if (counter >= 4) {
        return player == PLAYER_1_MARKER ? PLAYER_1_WON : PLAYER_2_WON;
    }

    // Next check col, go down from current piece position
    counter = 1;
    rowIter = row - 1;
    colIter = col;

    while (rowIter >= 0 && board[rowIter][colIter] == player) {
        ++counter;
        --rowIter;
    }

    // Then go up from current piece position
    rowIter = row + 1;

    while (rowIter < HEIGHT && board[rowIter][colIter] == player) {
        ++counter;
        ++rowIter;
    }

    // Check again if this made a win
    if (counter >= 4) {
        return player == PLAYER_1_MARKER ? PLAYER_1_WON : PLAYER_2_WON;
    }

    // Next check diagonal from bottom left to top right
    // expand towards bottom left from current piece first
    counter = 1;
    rowIter = row - 1;
    colIter = col - 1;

    while (rowIter >= 0 && colIter >= 0 && board[rowIter][colIter] == player) {
        ++counter;
        --rowIter;
        --colIter;
    }

    // Then expand from current piece to top right
    rowIter = row + 1;
    colIter = col + 1;

    while (rowIter < HEIGHT && colIter < WIDTH && board[rowIter][colIter] == player) {
        ++counter;
        ++rowIter;
        ++colIter;
    }

    // Check if there is a win in this diagonal
    if (counter >= 4) {
        return player == PLAYER_1_MARKER ? PLAYER_1_WON : PLAYER_2_WON;
    }

    // Next check diagonal from top left to bottom right
    // start by expanding to top left from current piece
    counter = 1;
    rowIter = row + 1;
    colIter = col - 1;

    while (rowIter < HEIGHT && colIter >= 0 && board[rowIter][colIter] == player) {
        ++counter;
        ++rowIter;
        --colIter;
    }

    // Then expand from current piece to bottom right
    rowIter = row - 1;
    colIter = col + 1;

    while (rowIter >= 0 && colIter < WIDTH && board[rowIter][colIter] == player) {
        ++counter;
        --rowIter;
        ++colIter;
    }

    // Check if there is a win in this diagonal
    if (counter >= 4) {
        return player == PLAYER_1_MARKER ? PLAYER_1_WON : PLAYER_2_WON;
    }

    // If not then nobody has won
    return NOT_FINISHED;
}

ConnectFourGameState::ConnectFourGameState(PlayerMarker startingPlayerMarker) :
        currentPlayerMarker(startingPlayerMarker),
        gameResult(NOT_FINISHED) {
    resetBoard();
}

ConnectFourGameState::ConnectFourGameState(const ConnectFourGameState &other) :
        currentPlayerMarker(other.currentPlayerMarker),
        lastAction(other.lastAction),
        gameResult(other.gameResult) {
    for (int col = 0; col < WIDTH; ++col) {
        for (int row = 0; row < HEIGHT; ++row)
            board[row][col] = other.board[row][col];
        colHeight[col] = other.getColHeight(col);
    }
}

ConnectFourGameState &ConnectFourGameState::operator=(const ConnectFourGameState &other) {
    for (int col = 0; col < WIDTH; ++col) {
        for (int row = 0; row < HEIGHT; ++row)
            board[row][col] = other.board[row][col];
        colHeight[col] = other.getColHeight(col);
    }

    currentPlayerMarker = other.currentPlayerMarker;
    lastAction = other.lastAction;
    gameResult = other.gameResult;

    return *this;
}

PlayerMarker ConnectFourGameState::getcurrentPlayerMarker() const {
    return currentPlayerMarker;
}

GameResult ConnectFourGameState::getGameResult() const {
    return gameResult;
}

void ConnectFourGameState::makeAction(const ConnectFourAction &action) {
    int row = getColHeight(action.col);

    // Check that the action is legal
    if (board[row][action.col] != EMPTY_MARKER)
        throw std::runtime_error("Illegal action: " + action.toString());

    // Make action
    board[row][action.col] = action.playerMarker;
    ++colHeight[action.col];
    lastAction = action;
}

bool ConnectFourGameState::isTerminal() const {
    return gameResult != NOT_FINISHED;
}

std::vector<ConnectFourAction> ConnectFourGameState::getLegalActions() const {
    std::vector<ConnectFourAction> actions;
    actions.reserve(WIDTH);

    for (int col = 0; col < WIDTH; ++col) {
        if (getColHeight(col) < HEIGHT) {
            actions.emplace_back(col, currentPlayerMarker);
        }
    }

    return actions;
}

ConnectFourAction ConnectFourGameState::getRandomAction() const {
    // Get legal actions
    auto legalActions = getLegalActions();

    // Check if there are legal actions
    if (legalActions.empty())
        throw std::runtime_error("No legal actions.");

    // Select random action
    return legalActions[RNG() % legalActions.size()];
}

ConnectFourGameState ConnectFourGameState::nextState(const ConnectFourAction &action) const {
    // Create new state from current
    ConnectFourGameState newState(*this);

    // Make action
    newState.makeAction(action);

    // Calculate game result
    newState.gameResult = newState.calculateGameResult();

    // Change the player turn
    newState.switchPlayer();

    return newState;
}

double ConnectFourGameState::rollout(PlayerMarker maximizingPlayer) const {
    GameResult result;

    if (isTerminal()) {
        result = getGameResult();
    } else {
        // Init simulation to current state
        ConnectFourGameState currentState(*this);
        ConnectFourAction action;

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

std::string ConnectFourGameState::toString() const {
    std::stringstream ss;

    for (int row = HEIGHT - 1; row >= 0; --row) {
        for (int col = 0; col < WIDTH; ++col)
            ss << board[row][col];
        ss << std::endl;
    }

    return ss.str();
}
