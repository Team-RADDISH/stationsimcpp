#---------------------------------------------------------------------------//
# Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
# Research Computing Services, University of Cambridge, UK
#
# Distributed under The MIT License (MIT)
# See accompanying file LICENSE
#---------------------------------------------------------------------------//

cmake_minimum_required(VERSION 3.15)
project(StationSim
        VERSION 1.0.0
        DESCRIPTION "Station simulator library.")

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    message(STATUS "USING OPTIONS FOR GNU COMPILER")
    set(MY_DEBUG_OPTIONS "-O0;-g;-Wall;-Wextra;-Wpedantic")
    set(MY_RELEASE_OPTIONS "-O3;-Wall;-Wextra;-Wpedantic")
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    message(STATUS "USING OPTIONS FOR MSVC COMPILER")
    set(MY_DEBUG_OPTIONS "/Wall /wd4710")
    set(MY_RELEASE_OPTIONS "/Wall;/wd4710;/wd4711;/wd4514")
endif ()

add_compile_options("$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>")
add_compile_options("$<$<CONFIG:RELEASE>:${MY_RELEASE_OPTIONS}>")

add_subdirectory(external)

add_library(StationSim SHARED)
add_dependencies(StationSim cxxplot-install)

target_sources(StationSim PRIVATE
        source/Model.cpp
        source/ModelParameters.cpp
        source/Agent.cpp
        source/ModelPlotting.cpp)
target_include_directories(StationSim PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR})

find_package(Python3 COMPONENTS Development NumPy)
target_link_libraries(StationSim PRIVATE Python3::Python Python3::NumPy)
find_package(cxxplot REQUIRED
        HINTS ${CMAKE_BINARY_DIR})
target_link_libraries(StationSim PRIVATE cxxplot::cxxplot)

include(GenerateExportHeader)
generate_export_header(StationSim)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        SomeProjConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

add_subdirectory(examples)
enable_testing()
add_subdirectory(tests)