target_sources(MCTS
        PUBLIC
        MCTS/Action.hpp
        MCTS/Agent.hpp
        MCTS/Node.hpp
        MCTS/State.hpp
        MCTS/Tree.hpp
        MCTS/Utils.hpp
        )

target_include_directories(MCTS PUBLIC ${CMAKE_CURRENT_LIST_DIR})
