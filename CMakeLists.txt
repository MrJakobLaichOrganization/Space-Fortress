cmake_minimum_required (VERSION 3.19)

# define a macro that helps defining an option
macro(game_set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

project("Game")

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

if(ENABLE_SANITIZERS)
    set(SFML_ENABLE_SANITIZERS ON)
    string(APPEND CMAKE_CXX_FLAGS " -fno-omit-frame-pointer -fno-sanitize-recover=all -fsanitize=undefined")
endif()

include(FetchContent)

# SFML Libraries
FetchContent_Declare(SFML
    SYSTEM 
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG dffdaa52a74428cf399502198a23ff2b2bdc8043)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(ImGui
    SYSTEM 
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.89.9)
FetchContent_MakeAvailable(ImGui)
FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)

set(IMGUI_SFML_FIND_SFML OFF)
FetchContent_Declare(ImGui-SFML
    SYSTEM 
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG 4bebb4ec4e87a26b8bfeda66f905bd93d70c2582)
FetchContent_MakeAvailable(ImGui-SFML)

# Cereal lib
FetchContent_Declare(Cereal
    SYSTEM 
    GIT_REPOSITORY https://github.com/USCiLab/cereal.git
    GIT_TAG v1.3.2)
set(SKIP_PERFORMANCE_COMPARISON ON)
set(BUILD_DOC OFF)
set(BUILD_SANDBOX OFF)
FetchContent_MakeAvailable(Cereal)
unset(SKIP_PERFORMANCE_COMPARISON)
unset(BUILD_DOC)
unset(BUILD_SANDBOX)

FetchContent_Declare(box2d
    SYSTEM 
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG main)
set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "")
set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "")
FetchContent_MakeAvailable(box2d)

file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable (Game ${SRC_FILES})
target_link_libraries(Game PRIVATE sfml-graphics ImGui-SFML::ImGui-SFML cereal box2d)
target_compile_features(Game PRIVATE cxx_std_20)
set_property(TARGET Game PROPERTY CXX_STANDARD 20)
target_include_directories(Game PRIVATE "./src/")

target_compile_definitions(Game PRIVATE ASSETS_DIR="${PROJECT_SOURCE_DIR}/assets")
target_compile_definitions(Game PRIVATE CONFIG_DIR="${PROJECT_SOURCE_DIR}/config")

sfml_set_option(CLANG_TIDY_EXECUTABLE clang-tidy STRING "Override clang-tidy executable, requires minimum version 14")
add_custom_target(tidy
    COMMAND ${CMAKE_COMMAND} -DCLANG_TIDY_EXECUTABLE=${CLANG_TIDY_EXECUTABLE} -DPROJECT_BINARY_DIR=${PROJECT_BINARY_DIR} -P ./cmake/Tidy.cmake
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} VERBATIM)