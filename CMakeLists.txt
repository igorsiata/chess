cmake_minimum_required(VERSION 3.24)
project(Chess)

set(CMAKE_CXX_STANDARD 17)

# add_compile_options(-Wall -Wextra -Werror -Wpedantic -pedantic-errors)

include_directories(include)

set(SOURCE_FILES src/pieces.cpp src/game.cpp src/move.cpp)

add_executable(Chess ${SOURCE_FILES} main.cpp)

set(SOURCES_FILES_TESTS test/test_position.cpp test/test_movement.cpp test/test_castle.cpp)

add_executable(Chess__test ${SOURCE_FILES} ${SOURCES_FILES_TESTS} test/main_gtest.cpp)

target_include_directories(Chess__test PUBLIC
        ../googletest-master/googlemock/include
        ../googletest-master/googletest/include
        )

add_subdirectory(../googletest-master googletest-master)

target_link_libraries(Chess__test gtest)

