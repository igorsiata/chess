cmake_minimum_required(VERSION 3.16)
project(Chess LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)

set(SOURCE_FILES src/pieces.cpp src/game.cpp src/move.cpp src/GUI.cpp)
set(SOURCES_FILES_TESTS test/test_position.cpp test/test_movement.cpp test/test_castle.cpp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

add_executable(Chess ${SOURCE_FILES} ${SOURCES_FILES_TESTS} main.cpp)

target_include_directories(Chess PUBLIC
        ../googletest-master/googlemock/include
        ../googletest-master/googletest/include
        )

add_subdirectory(../googletest-master googletest-master)


target_link_libraries(Chess PUBLIC gtest PRIVATE sfml-graphics)
target_compile_features(Chess PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
            TARGET Chess
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Chess>
            VERBATIM)
endif()

install(TARGETS Chess)