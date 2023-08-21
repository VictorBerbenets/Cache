cmake_minimum_required(VERSION 3.15)

project(lfu)

set(lfu_dir LFU_Cache)
set(perfect_dir Perfect_Caching)

add_executable(lfu ${lfu_dir}/src/main.cpp)
add_executable(perfect ${perfect_dir}/src/main.cpp)

set_target_properties(
    lfu perfect PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    COMPILE_FLAGS " -Wall -Wextra "
)

target_include_directories(
                            lfu
                            PRIVATE
                            ${CMAKE_CURRENT_SOURCE_DIR}/${lfu_dir}/include
)

target_include_directories(
                            perfect
                            PRIVATE
                            ${CMAKE_CURRENT_SOURCE_DIR}/${perfect_dir}/include
)


add_subdirectory(Compare)
add_subdirectory(tests)
