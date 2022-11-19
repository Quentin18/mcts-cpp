target_sources(uttt
        PRIVATE
            main.cpp
            uttt.cpp
        PUBLIC
            uttt.hpp
        )

target_include_directories(uttt
        PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}
        )

target_link_libraries(uttt
        PRIVATE
            MCTS
        )
