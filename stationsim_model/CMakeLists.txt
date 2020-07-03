#---------------------------------------------------------------------------//
# Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
# Research Computing Services, University of Cambridge, UK
#
# Distributed under The MIT License (MIT)
# See accompanying file LICENSE
#---------------------------------------------------------------------------//

add_library(StationSimModel SHARED)
target_sources(StationSimModel PUBLIC
        source/Agent.cpp
        source/Model.cpp
        source/ModelParameters.cpp
        source/ModelPlotting.cpp
        source/MultipleModelsRun.cpp
        source/MultipleModelsRunMPI.cpp
        source/Point2D.cpp)

target_include_directories(StationSimModel PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

set_target_properties(StationSimModel PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(StationSimModel PUBLIC ParticleFilter cxxplot Chronos Python3::Python Python3::NumPy)
target_include_directories(StationSimModel PUBLIC ${HDF5_CXX_INCLUDE_DIRS})
target_link_libraries(StationSimModel PUBLIC ${HDF5_CXX_LIBRARIES})
target_link_libraries(StationSimModel PUBLIC stationsim_project_options stationsim_project_warnings)
target_link_libraries(StationSimModel PUBLIC MPI::MPI_CXX)

if (STATIONSIM_ENABLE_OPENMP)
    find_package(OpenMP REQUIRED)
    if (OpenMP_CXX_FOUND)
        target_link_libraries(StationSimModel PUBLIC OpenMP::OpenMP_CXX)
    endif ()
endif ()

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        SomeProjConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)