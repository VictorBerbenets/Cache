cmake_minimum_required(VERSION 3.15)

project(compare)

add_executable(${PROJECT_NAME} src/main.cpp)

set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    COMPILE_FLAGS " -Wall -Wextra "
)

target_include_directories(     
                            ${PROJECT_NAME}
                            PRIVATE                            
                            ${CMAKE_CURRENT_SOURCE_DIR}/include
                            ${CMAKE_CURRENT_SOURCE_DIR}/../LFU_Cache/include
                            ${CMAKE_CURRENT_SOURCE_DIR}/../Perfect_Caching/include
)

