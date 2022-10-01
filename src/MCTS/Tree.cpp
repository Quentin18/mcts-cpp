/**
 * Manage game tree.
 * @author Quentin Deschamps (mailto:quentindeschamps18@gmail.com)
 * @date 2022
 */
#include <MCTS/Tree.hpp>

GameTree::GameTree(GameState *startingState, PlayerMarker maximizingPlayer) {
    root = new GameNode(startingState, maximizingPlayer);
}

GameTree::~GameTree() {
    delete root;
}

void GameTree::advanceTree(const GameAction *action) {
    GameNode *oldRoot = root;
    root = root->advanceTree(action);
    delete oldRoot;
}

GameState *GameTree::getCurrentState() const {
    return root->getCurrentState();
}

GameNode *GameTree::select(double c) {
    GameNode *node = root;
    while (!node->isTerminal()) {
        if (!node->isFullyExpanded())
            return node;
        else
            node = node->selectBestChild(c);
    }
    return node;
}

GameNode *GameTree::selectBestChild() {
    return root->selectBestChild(0.0);
}

void GameTree::growTree(uint maxIter, uint maxSeconds, bool debug) {
    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::duration<double> elapsedSeconds{};
    GameNode *node;
    uint iter = 0;

    // Start timer
    start = std::chrono::system_clock::now();

    // Grow tree
    do {
        // Select node to expand according to tree policy
        node = select();

        // Expand it: this will perform a rollout and back propagate the results
        node->expand();

        // Check if we need to stop
        ++iter;
        elapsedSeconds = std::chrono::system_clock::now() - start;
    } while (iter < maxIter && elapsedSeconds.count() < maxSeconds);

    if (debug)
        std::cout << "Made " << iter << " iterations in " << elapsedSeconds.count() << " seconds." << std::endl;
}

void GameTree::printStats() const {
    root->printStats();
}
