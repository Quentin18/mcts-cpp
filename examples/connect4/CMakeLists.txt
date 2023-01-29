target_sources(connect4
        PRIVATE
        main.cpp
        connect4.cpp
        PUBLIC
        connect4.hpp
        )

target_include_directories(connect4
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(connect4
        PRIVATE
        MCTS
        )
