/**
 * Manage game node.
 * @author Quentin Deschamps (mailto:quentindeschamps18@gmail.com)
 * @date 2022
 */
#include <MCTS/Node.hpp>

GameNode::GameNode(GameNode *parent, GameState *currentState, GameAction *lastAction, PlayerMarker maximizingPlayer) :
    parent(parent),
    currentState(currentState),
    lastAction(lastAction),
    numberOfSimulations(0),
    score(0.0),
    maximizingPlayer(maximizingPlayer) {
    untriedActions = currentState->getLegalActions();
    terminal = currentState->isTerminal();
}

GameNode::GameNode(GameState *currentState, PlayerMarker maximizingPlayer) :
    GameNode(nullptr, currentState, nullptr, maximizingPlayer) {}

GameNode::~GameNode() {
    delete currentState;
    delete lastAction;
    for (auto *child : children)
        delete child;
    for (auto *action : untriedActions)
        delete action;
}

GameNode *GameNode::advanceTree(const GameAction *action) {
    GameNode *nextNode;

    // Find the child corresponding to the action
    auto it = std::find_if(
            children.begin(),
            children.end(),
            [action](const GameNode *child) {
                return *(child->getLastAction()) == *action;
            }
    );

    // Check if we found the child, otherwise we create a new node
    if (it == children.end()) {
        std::cerr << "INFO: Child not found. Had to start over." << std::endl;
        nextNode = new GameNode(currentState->nextState(action), maximizingPlayer);
    } else {
        nextNode = *it;
    }

    // Reset parent to set the new node as root
    nextNode->parent = nullptr;

    return nextNode;
}

double GameNode::calculateWinRate(PlayerMarker player) const {
    if (player == maximizingPlayer)
        return score / numberOfSimulations;
    return 1.0 - score / numberOfSimulations;
}

double GameNode::uct(const GameNode *child, PlayerMarker player, double c) const {
    double exploration = c * sqrt(log((double) numberOfSimulations) / (double) child->numberOfSimulations);
    return child->calculateWinRate(player) + exploration;
}

GameNode *GameNode::selectBestChild(double c) const {
    // No children
    if (children.empty())
        throw std::runtime_error("No child to select.");

    // One child
    if (children.size() == 1)
        return children.at(0);

    // Return the best child according to the UCT policy
    PlayerMarker player = currentState->getcurrentPlayerMarker();
    return *std::max_element(
            children.begin(),
            children.end(),
            [this, c, player](const GameNode *firstChild, const GameNode *secondChild) {
                return this->uct(firstChild, player, c) < this->uct(secondChild, player, c);
            }
    );
}

GameState *GameNode::getCurrentState() const {
    return currentState;
}

GameAction *GameNode::getLastAction() const {
    return lastAction;
}

bool GameNode::isTerminal() const {
    return terminal;
}

bool GameNode::isFullyExpanded() const {
    return isTerminal() || untriedActions.empty();
}

void GameNode::expand() {
    if (isTerminal()) {
        rollout();
        return;
    } else if (isFullyExpanded()) {
        throw std::runtime_error("Cannot expand node.");
    }

    // Get next untried action
    GameAction *nextAction = untriedActions.front();
    untriedActions.erase(untriedActions.begin());

    // Get next state
    GameState *nextState = currentState->nextState(nextAction);

    // Create new node and add it to the tree
    auto *newNode = new GameNode(this, nextState, nextAction, maximizingPlayer);
    children.push_back(newNode);

    // Rollout and update stats
    newNode->rollout();
}

void GameNode::rollout() {
    double w = currentState->rollout(maximizingPlayer);
    backPropagate(w, 1);
}

void GameNode::backPropagate(double w, int n) {
    score += w;
    numberOfSimulations += n;
    if (parent != nullptr)
        parent->backPropagate(w, n);
}

void GameNode::printStats() {
    std::cout << "INFOS:" << std::endl;
    if (numberOfSimulations == 0) {
        std::cout << "Tree not expanded yet" << std::endl;
        return;
    }
    std::cout << "Number of simulations: " << numberOfSimulations << std::endl
        << "Branching factor at root: " << children.size() << std::endl
        << "Chances of player winning: " << std::setprecision(PRECISION)
        << 100.0 * (score / numberOfSimulations) << "%" << std::endl;

    // Sort children based on win rate of player's turn for this node
    PlayerMarker player = currentState->getcurrentPlayerMarker();
    std::sort(
            children.begin(),
            children.end(),
            [player](const GameNode *firstChild, const GameNode *secondChild) {
                return firstChild->calculateWinRate(player) > secondChild->calculateWinRate(player);
            });

    // Print best actions
    std::cout << "Best actions:" << std::endl;
    for (std::size_t i = 0; i < children.size(); ++i) {
        std::cout << i + 1 << ". "
            << children.at(i)->lastAction->toString()
            << " -> " << std::setprecision(PRECISION)
            << 100.0 * children.at(i)->calculateWinRate(player)
            << "%" << std::endl;
    }
}
