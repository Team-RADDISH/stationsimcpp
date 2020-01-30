#---------------------------------------------------------------------------//
# Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
# Research Computing Services, University of Cambridge, UK
#
# Distributed under The MIT License (MIT)
# See accompanying file LICENSE
#---------------------------------------------------------------------------//

add_executable(example_1)
target_sources(example_1 PRIVATE example_1.cpp)
target_include_directories(example_1 PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR})
target_link_libraries(example_1 PRIVATE
        StationSim Chronos)


add_executable(example_2)
target_sources(example_2 PRIVATE example_2.cpp)
target_include_directories(example_2 PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR})
target_link_libraries(example_2 PRIVATE
        StationSim Chronos)