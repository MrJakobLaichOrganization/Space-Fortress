cmake_minimum_required (VERSION 3.8)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Game" LANGUAGES CXX)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

# SFML Libraries
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(ImGui
    GIT_REPOSITORY https://github.com/ocornut/imgui
    GIT_TAG v1.89.9)
FetchContent_MakeAvailable(ImGui)
FetchContent_GetProperties(ImGui SOURCE_DIR IMGUI_DIR)

set(IMGUI_SFML_FIND_SFML OFF)
FetchContent_Declare(ImGui-SFML
    GIT_REPOSITORY https://github.com/SFML/imgui-sfml
    GIT_TAG 2.6.x)
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

file(GLOB_RECURSE SRC_FILES "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HDR_FILES "${PROJECT_SOURCE_DIR}/src/*.hpp")
add_executable (Game ${SRC_FILES} ${HDR_FILES})
target_link_libraries(Game PRIVATE sfml-graphics ImGui-SFML::ImGui-SFML cereal)
target_compile_features(Game PRIVATE cxx_std_20)
set_property(TARGET Game PROPERTY CXX_STANDARD 20)
target_include_directories(Game PRIVATE "./src/")

if(WIN32)
    add_custom_command(
        TARGET Game
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Game>
        VERBATIM)
endif()