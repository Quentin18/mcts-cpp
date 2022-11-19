target_sources(tictactoe
        PRIVATE
            main.cpp
            tictactoe.cpp
        PUBLIC
            tictactoe.hpp
        )

target_include_directories(tictactoe
        PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(tictactoe
        PRIVATE
            MCTS
        )
