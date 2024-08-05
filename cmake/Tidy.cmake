# Check executable exists
if(NOT EXISTS ${CLANG_TIDY_EXECUTABLE})
    find_program(CLANG_TIDY_EXEC_TEMP ${CLANG_TIDY_EXECUTABLE})
    if(CLANG_TIDY_EXEC_TEMP)
        set(CLANG_TIDY_EXECUTABLE ${CLANG_TIDY_EXEC_TEMP})
        unset(CLANG_TIDY_EXEC_TEMP)
    else()
        message(FATAL_ERROR "Unable to find clang-tidy executable: \"${CLANG_TIDY_EXECUTABLE}\"")
    endif()
endif()

# Check executable version
execute_process(COMMAND ${CLANG_TIDY_EXECUTABLE} --version OUTPUT_VARIABLE CLANG_TIDY_VERSION)
string(REGEX MATCH "version ([0-9]+)" CLANG_TIDY_VERSION ${CLANG_TIDY_VERSION})
unset(CLANG_TIDY_VERSION)
if(CMAKE_MATCH_1 GREATER_EQUAL 14)
    message(STATUS "Using clang-tidy version ${CMAKE_MATCH_1}")
else()
    message(FATAL_ERROR "clang-tidy version ${CMAKE_MATCH_1} is too low")
endif()

# Find Python and run-clang-tidy script
find_package(Python 3 REQUIRED)

find_program(RUN_CLANG_TIDY run-clang-tidy)
if(NOT RUN_CLANG_TIDY)
    message(FATAL_ERROR "Failed to find run-clang-tidy script")
endif()

# Run
execute_process(COMMAND ${Python_EXECUTABLE} ${RUN_CLANG_TIDY} -clang-tidy-binary ${CLANG_TIDY_EXECUTABLE} -quiet -p ${PROJECT_BINARY_DIR} "C:/Users/monst/Documents/projects/Space-Fortress/src/main.cpp" RESULTS_VARIABLE EXIT_CODE)
if(NOT EXIT_CODE STREQUAL 0)
    message(FATAL_ERROR "Analysis failed")
else()
    message (FATAL_ERROR "suc")
endif()
