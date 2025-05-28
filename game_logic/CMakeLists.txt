# Set the project name
project (game_logic)

# Add a library with the above sources

include_directories(include)

add_library(${PROJECT_NAME}
    game_logic
)
add_library(sub::game_logic ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

####### TESTS ############
add_executable(test_game_logic 

    )

target_include_directories(test_game_logic PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ../libs/googletest/googlemock/include
        ../libs/googletest/googletest/include
        )

add_subdirectory(../libs/googletest googletest-master)

target_link_libraries(test_game_logic game_logic gtest gmock)
