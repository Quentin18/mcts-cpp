/**
 * Tic Tac Toe Test.
 * @author Quentin Deschamps (mailto:quentindeschamps18@gmail.com)
 * @date 2022
 */
#include <gtest/gtest.h>

#include <MCTS/Agent.hpp>
#include "tictactoe.hpp"

#define MAX_ITER 100000
#define MAX_SECONDS 1
#define DEBUG false
#define NUM_GAMES 100

GameResult playTicTacToeGameMctsVsRandom() {
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

GameResult playTicTacToeGameMctsVsMcts() {
    TicTacToeGameState state(PLAYER_1_MARKER);
    TicTacToeAction action;
    MonteCarloTreeSearchAgent<TicTacToeAction, TicTacToeGameState> player1(
            state, PLAYER_1_MARKER, MAX_ITER, MAX_SECONDS, DEBUG);
    MonteCarloTreeSearchAgent<TicTacToeAction, TicTacToeGameState> player2(
            state, PLAYER_2_MARKER, MAX_ITER, MAX_SECONDS, DEBUG);

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


TEST(TicTacToeTest, MctsVsRandom) {
    srand(42);

    int mctsAgentWins = 0;

    for (int i = 0; i < NUM_GAMES; ++i) {
        switch (playTicTacToeGameMctsVsRandom()) {
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


TEST(TicTacToeTest, MctsVsMcts) {
    srand(42);

    int draws = 0;

    for (int i = 0; i < NUM_GAMES; ++i) {
        switch (playTicTacToeGameMctsVsMcts()) {
            case PLAYER_1_WON:
                std::cout << "MCTS agent 1 won" << std::endl;
                break;

            case PLAYER_2_WON:
                std::cout << "MCTS agent 2 won" << std::endl;
                break;

            default:
                std::cout << "No winner" << std::endl;
                ++draws;
                break;
        }
    }

    double drawsRate = (double) 100 * draws / NUM_GAMES;
    std::cout << "Draws rate is " << drawsRate << "%" << std::endl;
    EXPECT_GT(drawsRate, 95);
}
