# Set the project name
project (game_logic)

# Add a library with the above sources

include_directories(include)

add_library(${PROJECT_NAME}
    src/position_hasher.cpp
    src/fen_parser.cpp
    src/move_generator.cpp
    src/board_manager.cpp
    src/move_maker.cpp
)
add_library(sub::game_logic ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
target_compile_options(game_logic PRIVATE -O0 -g -Wall -Wextra -Wpedantic -Werror)
# target_compile_options(game_logic PRIVATE -O3)
####### TESTS ############
add_executable(test_game_logic 
    test/main_gtest.cpp
    test/main_test.cpp
    test/perft_test.cpp
    )

target_include_directories(test_game_logic PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ../libs/googletest/googlemock/include
        ../libs/googletest/googletest/include
        )

add_subdirectory(../libs/googletest googletest-master)

target_link_libraries(test_game_logic game_logic gtest gmock)
