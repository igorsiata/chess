# Set the project name
project (game_logic)

# Add a library with the above sources

include_directories(include)

add_library(${PROJECT_NAME}
    src/piece.cpp
    src/pawn.cpp
    src/knight.cpp
    src/sliding_piece.cpp
    src/king.cpp
    src/game_state.cpp
    src/game.cpp
    src/legal_moves_generator.cpp
)
add_library(sub::game_logic ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

####### TESTS ############
add_executable(test_game_logic 
    test/main_gtest.cpp
    test/test_piece.cpp
    test/test_pawn.cpp
    test/test_knight.cpp
    test/test_bishop.cpp
    test/test_rook.cpp
    test/test_queen.cpp
    test/test_king.cpp
    test/test_game_state.cpp
    test/test_legal_moves_generator.cpp
    test/test_number_of_positions.cpp
    # test/test_game.cpp
    )

target_include_directories(test_game_logic PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ../libs/googletest/googlemock/include
        ../libs/googletest/googletest/include
        )

add_subdirectory(../libs/googletest googletest-master)

target_link_libraries(test_game_logic game_logic gtest gmock)
