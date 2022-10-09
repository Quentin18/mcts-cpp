/**
 * Ultimate Tic Tac Toe game.
 * @author Quentin Deschamps (mailto:quentindeschamps18@gmail.com)
 * @date 2022
 */
#include "uttt.hpp"

UltimateTicTacToeAction::UltimateTicTacToeAction(int row, int col, PlayerMarker playerMarker) :
        row(row), col(col), playerMarker(playerMarker) {}

UltimateTicTacToeAction::UltimateTicTacToeAction(const UltimateTicTacToeAction &other) :
        row(other.row), col(other.col), playerMarker(other.playerMarker) {}

bool UltimateTicTacToeAction::operator==(const GameAction &other) const {
    // Cast action
    const auto &o = (const UltimateTicTacToeAction &) other;
    return row == o.row && col == o.col && playerMarker == o.playerMarker;
}

std::string UltimateTicTacToeAction::toString() const {
    std::stringstream ss;
    ss << "row = " << row << ", col = " << col << ", player = " << playerMarker;
    return ss.str();
}

GameResult TicTacToeGrid::calculateGameResult() const {
    // Check player 1 won
    if (playerWon(PLAYER_1_MARKER))
        return PLAYER_1_WON;

    // Check player 2 won
    if (playerWon(PLAYER_2_MARKER))
        return PLAYER_2_WON;

    // Check draw
    if (isFull())
        return DRAW;

    return NOT_FINISHED;
}

TicTacToeGrid::TicTacToeGrid() : gameResult(NOT_FINISHED) {
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            grid[row][col] = EMPTY_MARKER;
}

TicTacToeGrid::TicTacToeGrid(const TicTacToeGrid &other) : gameResult(other.gameResult) {
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            grid[row][col] = other.grid[row][col];
}

char TicTacToeGrid::get(int row, int col) const {
    return grid[row][col];
}

bool TicTacToeGrid::isEmpty(int row, int col) const {
    return grid[row][col] == EMPTY_MARKER;
}

bool TicTacToeGrid::isLegalAction(const UltimateTicTacToeAction *action) const {
    return isEmpty(action->row % 3, action->col % 3);
}

void TicTacToeGrid::makeAction(const GameAction *action) {
    // Cast action
    auto a = (UltimateTicTacToeAction *) action;

    // Check that the action is legal action
    if (!isLegalAction(a))
        throw std::runtime_error("Illegal action: " + a->toString());

    // Make action
    grid[a->row % 3][a->col % 3] = a->playerMarker;
}

void TicTacToeGrid::updateGameResult() {
    gameResult = calculateGameResult();
}

bool TicTacToeGrid::playerWon(PlayerMarker playerMarker) const {
    for (int i = 0; i < 3; i++) {
        // Check rows
        if (grid[i][0] == playerMarker && grid[i][1] == playerMarker && grid[i][2] == playerMarker)
            return true;

        // Check columns
        if (grid[0][i] == playerMarker && grid[1][i] == playerMarker && grid[2][i] == playerMarker)
            return true;
    }

    // Check diagonals
    return (
            (grid[0][0] == playerMarker && grid[1][1] == playerMarker && grid[2][2] == playerMarker) ||
            (grid[0][2] == playerMarker && grid[1][1] == playerMarker && grid[2][0] == playerMarker)
    );
}

bool TicTacToeGrid::isFull() const {
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            if (isEmpty(row, col))
                return false;

    return true;
}

GameResult TicTacToeGrid::getGameResult() const {
    return gameResult;
}

std::string TicTacToeGrid::toString() const {
    std::stringstream ss;

    ss << "+---+---+---+" << std::endl;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++)
            ss << "| " << grid[row][col] << " ";
        ss << "|" << std::endl << "+---+---+---+" << std::endl;
    }

    return ss.str();
}

TicTacToeGrid& TicTacToeGrid::operator=(const TicTacToeGrid &other) {
    gameResult = other.gameResult;
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            grid[row][col] = other.grid[row][col];

    return *this;
}

void UltimateTicTacToeGameState::switchPlayer() {
    currentPlayerMarker = (currentPlayerMarker == PLAYER_1_MARKER) ? PLAYER_2_MARKER : PLAYER_1_MARKER;
}

bool UltimateTicTacToeGameState::playerWon(PlayerMarker playerMarker) const {
    return masterGrid.playerWon(playerMarker);
}

GameResult UltimateTicTacToeGameState::calculateGameResult() const {
    // Check player 1 won
    if (playerWon(PLAYER_1_MARKER))
        return PLAYER_1_WON;

    // Check player 2 won
    if (playerWon(PLAYER_2_MARKER))
        return PLAYER_2_WON;

    // Check draw
    if (masterGrid.isFull())
        return DRAW;

    return NOT_FINISHED;
}

UltimateTicTacToeGameState::UltimateTicTacToeGameState(PlayerMarker startingPlayerMarker) :
        currentPlayerMarker(startingPlayerMarker),
        lastAction(nullptr),
        gameResult(NOT_FINISHED) {}

UltimateTicTacToeGameState::UltimateTicTacToeGameState(const UltimateTicTacToeGameState &other) :
        masterGrid(other.masterGrid),
        currentPlayerMarker(other.currentPlayerMarker),
        lastAction(other.lastAction),
        gameResult(other.gameResult) {
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            smallGrids[row][col] = other.smallGrids[row][col];
}

UltimateTicTacToeGameState::~UltimateTicTacToeGameState() {
    delete lastAction;
}

PlayerMarker UltimateTicTacToeGameState::getcurrentPlayerMarker() const {
    return currentPlayerMarker;
}

GameResult UltimateTicTacToeGameState::getGameResult() const {
    return gameResult;
}

bool UltimateTicTacToeGameState::isLegalAction(const UltimateTicTacToeAction *action) const {
    int gridRowToPlay, gridColToPlay, gridRowPlayed, gridColPlayed;
    bool freeGrid = true;
    gridRowPlayed = action->row / 3;
    gridColPlayed = action->col / 3;

    // Get small grid to play
    if (lastAction != nullptr && smallGrids[lastAction->row % 3][lastAction->col % 3].getGameResult() == NOT_FINISHED) {
        gridRowToPlay = lastAction->row % 3;
        gridColToPlay = lastAction->col % 3;
        freeGrid = false;
    }

    // Check if the grid is valid
    if (!freeGrid && (gridRowToPlay != gridRowPlayed || gridColToPlay != gridColPlayed))
        return false;

    // Check if the position is available
    return smallGrids[gridRowPlayed][gridColPlayed].isLegalAction(action);
}

void UltimateTicTacToeGameState::makeAction(const GameAction *action) {
    // Cast action
    auto a = (UltimateTicTacToeAction *) action;

    // Check that the action is legal
    if (!isLegalAction(a))
        throw std::runtime_error("Illegal action: " + a->toString());

    int gridRowPlayed = a->row / 3;
    int gridColPlayed = a->col / 3;

    // Update small grid
    TicTacToeGrid *grid = &smallGrids[gridRowPlayed][gridColPlayed];
    grid->makeAction(a);
    grid->updateGameResult();

    // Update master grid
    GameResult result = grid->getGameResult();
    if (result != NOT_FINISHED) {
        PlayerMarker marker = (result == PLAYER_1_WON) ? PLAYER_1_MARKER : (result == PLAYER_2_WON) ? PLAYER_2_MARKER : DRAW_MARKER;
        auto masterAction = UltimateTicTacToeAction(gridRowPlayed, gridColPlayed, marker);
        masterGrid.makeAction(&masterAction);
        masterGrid.updateGameResult();
    }
}

bool UltimateTicTacToeGameState::isTerminal() const {
    return gameResult != NOT_FINISHED;
}

std::vector<GameAction *> UltimateTicTacToeGameState::getLegalActions() const {
    std::vector<GameAction *> actions;
    const TicTacToeGrid *grid;

    if (lastAction != nullptr && smallGrids[lastAction->row % 3][lastAction->col % 3].getGameResult() == NOT_FINISHED) {
        grid = &smallGrids[lastAction->row % 3][lastAction->col % 3];
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                if (grid->isEmpty(row, col))
                    actions.push_back(new UltimateTicTacToeAction((lastAction->row % 3) * 3 + row,
                                                                  (lastAction->col % 3) * 3 + col,
                                                                  currentPlayerMarker));
            }
        }
    } else {
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                grid = &smallGrids[row][col];
                if (grid->getGameResult() == NOT_FINISHED) {
                    for (int r = 0; r < 3; r++) {
                        for (int c = 0; c < 3; c++) {
                            if (grid->isEmpty(r, c))
                                actions.push_back(new UltimateTicTacToeAction((row % 3) * 3 + r,
                                                                              (col % 3) * 3 + c,
                                                                              currentPlayerMarker));
                        }
                    }
                }
            }
        }
    }

    return actions;
}

GameAction *UltimateTicTacToeGameState::getRandomAction() const {
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

GameState *UltimateTicTacToeGameState::nextState(const GameAction *action) const {
    // Create new state from current
    auto newState = new UltimateTicTacToeGameState(*this);

    // Make action
    newState->makeAction(action);

    // Update last action
    newState->lastAction = new UltimateTicTacToeAction(*((UltimateTicTacToeAction *) action));

    // Calculate game result
    newState->gameResult = newState->calculateGameResult();

    // Change the player turn
    newState->switchPlayer();

    return newState;
}

double UltimateTicTacToeGameState::rollout(PlayerMarker maximizingPlayer) const {
    GameResult result;

    if (isTerminal()) {
        result = getGameResult();
    } else {
        // Init simulation to current state
        auto currentState = new UltimateTicTacToeGameState(*this);
        GameAction *action;

        // Run game until we reach a terminal state
        do {
            action = currentState->getRandomAction();
            currentState->makeAction(action);
            currentState->lastAction = new UltimateTicTacToeAction(*((UltimateTicTacToeAction *) action));
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

std::string UltimateTicTacToeGameState::toString() const {
    std::stringstream ss;

    for (int row = 0; row < 9; row++) {
        ss << (row % 3 == 0 ? "+===+===+===+===+===+===+===+===+===+" : "+---+---+---+---+---+---+---+---+---+") << std::endl;
        for (int col = 0; col < 9; col++) {
            ss << (col % 3 == 0 ? "| " : ": ") << smallGrids[row / 3][col / 3].get(row % 3, col % 3) << " ";
        }
        ss << "|" << std::endl;
    }
    ss << "+===+===+===+===+===+===+===+===+===+" << std::endl;

    // Display master grid
    ss << "Master grid :" << std::endl << masterGrid.toString();

    return ss.str();
}
