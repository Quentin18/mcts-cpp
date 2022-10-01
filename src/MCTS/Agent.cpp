/**
 * Manage game agents.
 * @author Quentin Deschamps (mailto:quentindeschamps18@gmail.com)
 * @date 2022
 */
#include <MCTS/Agent.hpp>

MonteCarloTreeSearchAgent::MonteCarloTreeSearchAgent(
        GameState *startingState,
        PlayerMarker player,
        uint maxIter,
        uint maxSeconds,
        bool debug) : player(player), maxIter(maxIter), maxSeconds(maxSeconds), debug(debug) {
    tree = new GameTree(startingState, player);
}

MonteCarloTreeSearchAgent::~MonteCarloTreeSearchAgent() {
    delete tree;
}

GameAction *MonteCarloTreeSearchAgent::getAction(const GameState *state, const GameAction *lastAction) {
    GameNode *bestChild;
    GameAction *bestAction;

    // Print legal actions
    if (debug) {
        std::cout << "Legal actions:" << std::endl;
        for (const auto *action : state->getLegalActions()) {
            std::cout << action->toString() << std::endl;
            delete action;
        }
    }

    // Advance the tree with last action
    if (lastAction != nullptr)
        tree->advanceTree(lastAction);

    // If we have a terminal state, we cannot do anything
    if (tree->getCurrentState()->isTerminal())
        throw std::invalid_argument("The current state is terminal.");

    // Grow the tree
    tree->growTree(maxIter, maxSeconds, debug);

    // Print stats
    if (debug)
        tree->printStats();

    // Select the best child
    bestChild = tree->selectBestChild();
    if (bestChild == nullptr)
        throw std::invalid_argument("Tree root has no children.");

    // Get corresponding action
    bestAction = bestChild->getLastAction();
    std::cout << "MCTS action selected: " << bestAction->toString() << std::endl;

    // Advance tree with the chosen action
    tree->advanceTree(bestAction);

    return bestAction;
}

GameAction *RandomAgent::getAction(const GameState *state, const GameAction *lastAction) {
    GameAction *randomAction;
    uint actionIndex;

    // Get legal actions
    auto legalActions = state->getLegalActions();

    // Check if there are legal actions
    if (legalActions.empty())
        throw std::runtime_error("No legal actions.");

    // Select random action
    actionIndex = rand() % legalActions.size();
    for (uint i = 0; i < legalActions.size(); ++i) {
        if (i == actionIndex)
            randomAction = legalActions[i];
        else
            delete legalActions[i];
    }

    std::cout << "Random action selected: " << randomAction->toString() << std::endl;

    return randomAction;
}
