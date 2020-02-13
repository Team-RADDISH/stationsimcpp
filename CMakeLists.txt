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

option(ENABLE_OPENMP "Enables OpenMP" OFF)
option(ENABLE_MPI "Enables MPI" OFF)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    message(STATUS "USING OPTIONS FOR GNU COMPILER")
    set(MY_DEBUG_OPTIONS "-march=native;-O0;-g;-Wall;-Wextra;-Wpedantic")
    set(MY_RELEASE_OPTIONS "-march=native;-O3;-Wall;-Wextra;-Wpedantic")
endif ()

if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    message(STATUS "USING OPTIONS FOR MSVC COMPILER")
    set(MY_DEBUG_OPTIONS "/Wall /wd4710")
    set(MY_RELEASE_OPTIONS "/Wall;/wd4710;/wd4711;/wd4514")
endif ()

add_library(StationSim SHARED)
target_compile_options(StationSim PUBLIC "$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>")
target_compile_options(StationSim PUBLIC "$<$<CONFIG:RELEASE>:${MY_RELEASE_OPTIONS}>")

target_sources(StationSim PRIVATE
        source/Model.cpp
        source/ModelParameters.cpp
        source/MultipleModelsRun.cpp
        source/Agent.cpp
        source/ModelPlotting.cpp
        source/HelpFunctions.cpp
        source/Point2D.cpp)
target_include_directories(StationSim PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR})

find_package(HDF5 REQUIRED COMPONENTS CXX)
if (${HDF5_FOUND})
    message(STATUS "HDF5 FOUND!")
    message(STATUS ${HDF5_CXX_INCLUDE_DIRS})
    message(STATUS ${HDF5_CXX_LIBRARIES})
endif ()

find_package(Python3 COMPONENTS Development NumPy)
target_link_libraries(StationSim PRIVATE Python3::Python Python3::NumPy)
target_include_directories(StationSim PRIVATE ${HDF5_CXX_INCLUDE_DIRS})
target_link_libraries(StationSim PRIVATE ${HDF5_CXX_LIBRARIES})

if (ENABLE_OPENMP)
    find_package(OpenMP REQUIRED)
    if (OpenMP_CXX_FOUND)
        target_link_libraries(StationSim PUBLIC OpenMP::OpenMP_CXX)
        target_link_libraries(StationSim PUBLIC OpenMP::OpenMP_CXX)
        target_link_libraries(StationSim PUBLIC OpenMP::OpenMP_CXX)
    endif ()
endif ()

if (ENABLE_MPI)
    find_package(MPI REQUIRED)
    if (MPI_FOUND)
        message(STATUS "USING MPI")

        target_link_libraries(StationSim PUBLIC MPI::MPI_CXX)
        target_link_libraries(StationSim PUBLIC MPI::MPI_CXX)
        target_link_libraries(StationSim PUBLIC MPI::MPI_CXX)

        target_sources(StationSim PRIVATE
                source/MultipleModelsRunMPI.cpp)
    endif ()
endif ()

add_subdirectory(external)

target_link_libraries(StationSim PRIVATE cxxplot)
target_link_libraries(StationSim PRIVATE Chronos)

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