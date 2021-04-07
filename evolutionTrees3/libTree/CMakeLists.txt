cmake_minimum_required(VERSION 3.17)
set(CLP_DIRECTORY "$ENV{HOME}/CLionProjects")
project(devTree LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_DEBUG_POSTFIX _d)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

add_subdirectory("${CLP_DIRECTORY}/ch" build)
add_library(Tree src/Field.cpp src/Seed.cpp src/TreeBase.cpp src/Tree.cpp src/Area.cpp)
target_include_directories(Tree PUBLIC
        include)
target_link_libraries(Tree PUBLIC
        ch)
set_target_properties(Tree
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/../lib"
        LIBRARY_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/../lib"
        )

add_executable(${PROJECT_NAME}Tests tests.cpp)
target_link_libraries(${PROJECT_NAME}Tests PUBLIC
        Tree gtest gtest_main)

add_executable(${PROJECT_NAME}Console main.cpp)
target_link_libraries(${PROJECT_NAME}Console
        PUBLIC Tree)
