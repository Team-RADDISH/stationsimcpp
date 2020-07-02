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

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)

# General options
option(STATIONSIM_ENABLE_EXAMPLES "Enables examples" ON)
option(STATIONSIM_ENABLE_TESTS "Enables tests" ON)
option(STATIONSIM_ENABLE_OPENMP "Enables OpenMP" OFF)
option(STATIONSIM_ENABLE_MPI "Enables MPI" OFF)
option(STATIONSIM_ENABLE_OPENACC "Enables OpenACC" OFF)

# Project dependencies
include(cmake/ProjectDependencies.cmake)

# Project settings
include(cmake/StandardProjectSettings.cmake)
add_library(stationsim_project_options INTERFACE)
target_compile_features(stationsim_project_options INTERFACE cxx_std_17)

if (MPI_CXX_FOUND)
    target_link_libraries(stationsim_project_options INTERFACE MPI::MPI_CXX)
endif ()

# Standard compiler warnings
add_library(stationsim_project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(stationsim_project_warnings)

add_subdirectory(external)
add_subdirectory(source)
include(GenerateExportHeader)
generate_export_header(StationSim)

add_subdirectory(examples)
enable_testing()
add_subdirectory(tests)