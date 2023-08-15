cmake_minimum_required(VERSION 2.8)

project(lfu_cache)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

#file(GLOB 
#       CPPS "src/*.cpp")
#Or set(SOURCES main.cpp)
#add_executable(${PROJECT_NAME} ${SOURCES})

include_directories("${PROJECT_SOURCE_DIR}/include")

add_executable(${PROJECT_NAME} src/main.cpp)
#add_executable(${PROJECT_NAME} ${CPPS})
