cmake_minimum_required(VERSION 3.15)

project(examples)

cmake_minimum_required(VERSION 3.15.3)
add_executable(example_1)
target_sources(example_1 PRIVATE example_1.cpp)
target_include_directories(example_1 PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR})
target_link_libraries(example_1 PRIVATE
        StationSim)