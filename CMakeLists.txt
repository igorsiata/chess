cmake_minimum_required(VERSION 3.24)
project(Chess)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors)

include_directories(include)

set(SOURCE_FILES src/pieces.cpp src/chessboard.cpp src/game.cpp)

add_executable(Chess ${SOURCE_FILES} main.cpp)

set(SOURCES_FILES_TESTS)

add_executable(Chess__test ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)

target_include_directories(Chess__test PUBLIC
        ../googletest-master/googlemock/include
        ../googletest-master/googletest/include
        )

add_subdirectory(../googletest-master googletest-master)

target_link_libraries(Chess__test gmock)

target_compile_definitions(Chess__test PUBLIC EXERCISE_ID=EXERCISE_ID_FACTORY)