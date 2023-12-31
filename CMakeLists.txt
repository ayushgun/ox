cmake_minimum_required(VERSION 3.27)
project(ox)

set(CMAKE_CXX_STANDARD 20)

# Compiler flags for Debug build
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(DEV_WARNINGS "-Wall -Wextra -Werror -pedantic")
    set(SANITIZERS "-fsanitize=undefined -fsanitize=address")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${DEV_WARNINGS} ${SANITIZERS}")
endif ()

# Set the include directory
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

# Add the main executable
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(ox ${SOURCES})
target_include_directories(ox PRIVATE ${INCLUDE_DIR})

# Find GTest package
find_package(GTest REQUIRED)
if(GTEST_FOUND)
    enable_testing()

    # Gather all source files but exclude the main.cpp
    file(GLOB_RECURSE TEST_SOURCES "src/*.cpp")
    list(REMOVE_ITEM TEST_SOURCES "${PROJECT_SOURCE_DIR}/src/main.cpp")

    # Add the test executable
    add_executable(OXTests test/BookTest.cpp ${TEST_SOURCES})
    target_link_libraries(OXTests GTest::GTest GTest::Main)
    target_include_directories(OXTests PRIVATE ${INCLUDE_DIR} ${GTEST_INCLUDE_DIRS})
    include(GoogleTest)
    gtest_discover_tests(OXTests)
endif()
