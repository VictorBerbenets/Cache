cmake_minimum_required(VERSION 3.22.1)

project(testing)

add_executable(${PROJECT_NAME} src/main.cpp)
add_executable(another src/main.cpp)

target_include_directories(
                            ${PROJECT_NAME}
                            PRIVATE
                            ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(
                            another 
                            PRIVATE
                            ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(
                           ${PROJECT_NAME}
                           PRIVATE
                           PERFECT_TEST_GENERATING_
)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    COMPILE_FLAGS " -Wall -Wextra "
)

