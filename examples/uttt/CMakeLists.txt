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
            ${CMAKE_SOURCE_DIR}/src
        )

target_link_libraries(uttt
        PRIVATE
            MCTS
        )
