/**
 * Ultimate Tic Tac Toe game.
 * @author Quentin Deschamps (mailto:quentindeschamps18@gmail.com)
 * @date 2022
 */
#include <MCTS/Agent.hpp>

#include "uttt.hpp"

#define MAX_ITER 100000
#define MAX_SECONDS 5
#define DEBUG true

/**
 * Main function for Ultimate Tic Tac Toe game.
 */
int main() {
    // Init random number generator
    srand(time(nullptr));

    std::cout << "Welcome to the Ultimate Tic Tac Toe game!" << std::endl;

    // Init game state
    GameState *state, *nextState;
    state = new UltimateTicTacToeGameState(PLAYER_1_MARKER);
    std::cout << state->toString() << std::endl;

    // Init game action
    GameAction *lastAction, *action;
    lastAction = nullptr;

    // Init agents
    MonteCarloTreeSearchAgent player1(new UltimateTicTacToeGameState(*(UltimateTicTacToeGameState *) state),
                                      PLAYER_1_MARKER, MAX_ITER, MAX_SECONDS, DEBUG);
    RandomAgent player2;

    // Game loop
    while (true) {
        // Player 1 plays
        action = player1.getAction(state, lastAction);
        delete lastAction;
        lastAction = new UltimateTicTacToeAction(*(UltimateTicTacToeAction *) action);
        nextState = state->nextState(lastAction);
        delete state;
        state = nextState;
        std::cout << state->toString() << std::endl;
        if (state->isTerminal())
            break;

        // Player 2 plays
        action = player2.getAction(state, lastAction);
        delete lastAction;
        lastAction = action;
        nextState = state->nextState(lastAction);
        delete state;
        state = nextState;
        std::cout << state->toString() << std::endl;
        if (state->isTerminal())
            break;
    }

    // Display game result
    switch (state->getGameResult()) {
        case PLAYER_1_WON:
            std::cout << "Player " << PLAYER_1_MARKER << " won" << std::endl;
            break;

        case PLAYER_2_WON:
            std::cout << "Player " << PLAYER_2_MARKER << " won" << std::endl;
            break;

        default:
            std::cout << "No winner" << std::endl;
            break;
    }

    // Free memory
    delete state;
    delete lastAction;

    return EXIT_SUCCESS;
}
