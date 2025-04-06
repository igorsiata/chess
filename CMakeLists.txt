cmake_minimum_required(VERSION 3.16)
project(Chess LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(game_logic)
# add_subdirectory(GUI)

# add_executable(Chess ${SOURCE_FILES}
#         main.cpp)

# target_include_directories(Chess PUBLIC
#         ../googletest-master/googlemock/include
#         ../googletest-master/googletest/include
#         )

# add_subdirectory(../googletest-master googletest-master)
