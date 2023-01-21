/**
 * Tic Tac Toe Test.
 * @author Quentin Deschamps (mailto:quentindeschamps18@gmail.com)
 * @date 2022
 */
#include <gtest/gtest.h>

#include <MCTS/Agent.hpp>
#include "tictactoe.hpp"

#define MAX_ITER 100000
#define MAX_SECONDS 2
#define DEBUG false
#define NUM_GAMES 100

GameResult playTicTacToeGame() {
    TicTacToeGameState state(PLAYER_1_MARKER);
    TicTacToeAction action;
    MonteCarloTreeSearchAgent<TicTacToeAction, TicTacToeGameState> player1(
            state, PLAYER_1_MARKER, MAX_ITER, MAX_SECONDS, DEBUG);
    RandomAgent<TicTacToeAction, TicTacToeGameState> player2;

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

TEST(TicTacToeTest, MctsVsRandom)
{
    srand(42);

    int mctsAgentWins = 0;

    for (int i = 0; i < NUM_GAMES; ++i) {
        switch (playTicTacToeGame()) {
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
