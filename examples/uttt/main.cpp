/**
 * Ultimate Tic Tac Toe game.
 * @author Quentin Deschamps (mailto:quentindeschamps18@gmail.com)
 * @date 2022
 */
#include <MCTS/Agent.hpp>

#include "uttt.hpp"

#define MAX_ITER 100000
#define MAX_SECONDS 3
#define DEBUG true

/**
 * Main function for Ultimate Tic Tac Toe game.
 */
int main() {
    // Init random number generator
    srand(time(nullptr));

    std::cout << "Welcome to the Ultimate Tic Tac Toe game!" << std::endl;

    // Init game state
    UltimateTicTacToeGameState state(PLAYER_1_MARKER);
    std::cout << state.toString() << std::endl;

    // Init game action
    UltimateTicTacToeAction action;

    // Init agents
    MonteCarloTreeSearchAgent<UltimateTicTacToeAction, UltimateTicTacToeGameState> player1(
            state, PLAYER_1_MARKER, MAX_ITER, MAX_SECONDS, DEBUG);
    RandomAgent<UltimateTicTacToeAction, UltimateTicTacToeGameState> player2;

    // Game loop
    while (true) {
        // Player 1 plays
        action = player1.getAction(state, action);
        state = state.nextState(action);
        std::cout << state.toString() << std::endl;
        if (state.isTerminal())
            break;

        // Player 2 plays
        action = player2.getAction(state, action);
        state = state.nextState(action);
        std::cout << state.toString() << std::endl;
        if (state.isTerminal())
            break;
    }

    // Display game result
    switch (state.getGameResult()) {
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

    return EXIT_SUCCESS;
}
