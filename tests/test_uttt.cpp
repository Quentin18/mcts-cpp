/**
 * Ultimate Tic Tac Toe Test.
 * @author Quentin Deschamps (mailto:quentindeschamps18@gmail.com)
 * @date 2022
 */
#include <gtest/gtest.h>

#include <MCTS/Agent.hpp>
#include "uttt.hpp"

#define MAX_ITER 100000
#define MAX_SECONDS 1
#define DEBUG false
#define NUM_GAMES 10

GameResult playUltimateTicTacToeGame() {
    UltimateTicTacToeGameState state(PLAYER_1_MARKER);
    UltimateTicTacToeAction action;
    MonteCarloTreeSearchAgent<UltimateTicTacToeAction, UltimateTicTacToeGameState> player1(
            state, PLAYER_1_MARKER, MAX_ITER, MAX_SECONDS, DEBUG);
    RandomAgent<UltimateTicTacToeAction, UltimateTicTacToeGameState> player2;

    while (true) {
        action = player1.getAction(state, action);
        state = state.nextState(action);
        if (state.isTerminal())
            break;

        action = player2.getAction(state, action);
        state = state.nextState(action);
        if (state.isTerminal())
            break;
    }

    return state.getGameResult();
}

TEST(UltimateTicTacToeTest, MctsVsRandom) {
    srand(42);

    int mctsAgentWins = 0;

    for (int i = 0; i < NUM_GAMES; ++i) {
        switch (playUltimateTicTacToeGame()) {
            case PLAYER_1_WON:
                std::cout << "MCTS agent won" << std::endl;
                ++mctsAgentWins;
                break;

            case PLAYER_2_WON:
                std::cout << "Random agent won" << std::endl;
                break;

            default:
                std::cout << "No winner" << std::endl;
                break;
        }
    }

    double mctsWinRate = (double) 100 * mctsAgentWins / NUM_GAMES;
    std::cout << "MCTS win rate is " << mctsWinRate << "%" << std::endl;
    EXPECT_GT(mctsWinRate, 95);
}
