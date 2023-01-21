include(FetchContent)
FetchContent_Declare(gtest
        QUIET
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
        )
# configure build of googletest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(gtest)

add_executable(unit_tests)

target_sources(unit_tests
        PRIVATE
            test_tictactoe.cpp
            test_uttt.cpp
            ${CMAKE_SOURCE_DIR}/examples/tictactoe/tictactoe.cpp
            ${CMAKE_SOURCE_DIR}/examples/uttt/uttt.cpp
        PUBLIC
            ${CMAKE_SOURCE_DIR}/examples/tictactoe/tictactoe.hpp
            ${CMAKE_SOURCE_DIR}/examples/uttt/uttt.hpp
        )

target_include_directories(unit_tests
        PUBLIC
            ${CMAKE_CURRENT_LIST_DIR}
            ${CMAKE_SOURCE_DIR}/examples/tictactoe
            ${CMAKE_SOURCE_DIR}/examples/uttt
        )

target_link_libraries(unit_tests
        PRIVATE
            MCTS
            gtest_main
        )

# automatic discovery of unit tests
include(GoogleTest)
gtest_discover_tests(unit_tests
        PROPERTIES
            LABELS "unit"
        DISCOVERY_TIMEOUT  # how long to wait (in seconds) before crashing
            240
        )
