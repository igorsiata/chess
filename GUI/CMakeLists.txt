# Set the project name
project (GUI)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

include_directories(include)

set(SOURCE_FILES
    src/game_interface.cpp
    src/GUI.cpp)


add_executable(Chess ${SOURCE_FILES}
    main.cpp)

target_link_libraries(Chess PRIVATE sfml-graphics sub::game_logic)
target_compile_features(Chess PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
            TARGET Chess
            COMMENT "Copy OpenAL DLL"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Chess>
            VERBATIM)
endif()

install(TARGETS Chess)

add_custom_command(TARGET Chess POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/textures $<TARGET_FILE_DIR:Chess>/textures)

