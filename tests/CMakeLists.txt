cmake_minimum_required(VERSION 3.22.1)

project(testing)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")


include_directories("${PROJECT_SOURCE_DIR}/include")

add_executable(${PROJECT_NAME} src/main.cpp)
