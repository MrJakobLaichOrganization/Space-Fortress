cmake_minimum_required (VERSION 3.19)

set(VERBOSE=1)

# define a macro that helps defining an option
macro(game_set_option var default type docstring)
    if(NOT DEFINED ${var})
        set(${var} ${default})
    endif()
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

project ("Game" LANGUAGES CXX)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

# SFML Libraries
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG dffdaa52a74428cf399502198a23ff2b2bdc8043)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.89.9)
FetchContent_MakeAvailable(ImGui)
FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)

set(IMGUI_SFML_FIND_SFML OFF)
FetchContent_Declare(ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG 4bebb4ec4e87a26b8bfeda66f905bd93d70c2582)
FetchContent_MakeAvailable(ImGui-SFML)

# Cereal lib
FetchContent_Declare(Cereal
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
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG main)
FetchContent_MakeAvailable(box2d)

file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable (Game ${SRC_FILES})
target_link_libraries(Game PRIVATE sfml-graphics ImGui-SFML::ImGui-SFML cereal box2d)
target_compile_features(Game PRIVATE cxx_std_20)
set_property(TARGET Game PROPERTY CXX_STANDARD 20)
target_include_directories(Game PRIVATE "./src/")

target_compile_definitions(Game PRIVATE ASSETS_DIR="${PROJECT_SOURCE_DIR}/assets")

game_set_option(CLANG_FORMAT_EXECUTABLE clang-format STRING "Override clang-format executable, requires version 12, 13, or 14")
add_custom_target(format
    COMMAND ${CMAKE_COMMAND} -DCLANG_FORMAT_EXECUTABLE=${CLANG_FORMAT_EXECUTABLE} -P ./cmake/Format.cmake
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} VERBATIM)

game_set_option(CLANG_TIDY_EXECUTABLE clang-tidy STRING "Override clang-tidy executable, requires minimum version 14")


# Check executable exists
if(NOT EXISTS ${CLANG_TIDY_EXECUTABLE})
    find_program(CLANG_TIDY_EXEC_TEMP ${CLANG_TIDY_EXECUTABLE})
    if(CLANG_TIDY_EXEC_TEMP)
        set(CLANG_TIDY_EXECUTABLE ${CLANG_TIDY_EXEC_TEMP})
        unset(CLANG_TIDY_EXEC_TEMP)
    endif()
endif()

# Find Python and run-clang-tidy script
find_package(Python 3 REQUIRED)

#find_program(RUN_CLANG_TIDY run-clang-tidy)
#set(RUN_CLANG_TIDY run-clang-tidy)
#set(CLANG_TIDY_EXECUTABLE "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/Llvm/x64/bin/clang-tidy.exe")


add_custom_target(tidy
    COMMAND ${Python_EXECUTABLE} ${RUN_CLANG_TIDY} -clang-tidy-binary ${CLANG_TIDY_EXECUTABLE} -p ${PROJECT_BINARY_DIR} ${SRC_FILES}
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} VERBATIM)