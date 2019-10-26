
cmake_minimum_required(VERSION 2.8)


project(Game)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

include_directories("${PROJECT_SOURCE_DIR}")

set(CMAKE_CXX_FLAGS "-std=c++11 -Wall")

add_executable(Game main.cpp)
target_link_libraries(Game ${GTEST_LIBRARIES} pthread)
