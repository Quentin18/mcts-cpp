add_executable(tictactoe)

target_sources(tictactoe
        PRIVATE
            tictactoe/main.cpp
            tictactoe/tictactoe.cpp
        PUBLIC
            tictactoe/tictactoe.hpp
        )

target_include_directories(tictactoe
        PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}
            ${CMAKE_SOURCE_DIR}/src
        )

target_link_libraries(tictactoe
        PRIVATE
            MCTS
        )
