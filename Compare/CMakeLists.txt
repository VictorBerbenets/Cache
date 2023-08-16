cmake_minimum_required(VERSION 3.22.1)

project(compare)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y") 
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

include_directories( "${PROJECT_SOURCE_DIR}/../LFU_Cache/include"
                     "${PROJECT_SOURCE_DIR}/../Perfect_Caching/include"
                     "${PROJECT_SOURCE_DIR}/include")

                     #set(main src/main.cpp)
add_executable(${PROJECT_NAME} src/main.cpp)
  

