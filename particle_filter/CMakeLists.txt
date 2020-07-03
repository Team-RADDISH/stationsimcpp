#---------------------------------------------------------------------------//
# Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
# Research Computing Services, University of Cambridge, UK
#
# Distributed under The MIT License (MIT)
# See accompanying file LICENSE
#---------------------------------------------------------------------------//

add_library(ParticleFilter INTERFACE)

target_include_directories(ParticleFilter INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(ParticleFilter INTERFACE cxxplot Chronos Python3::Python Python3::NumPy)
target_include_directories(ParticleFilter INTERFACE ${HDF5_CXX_INCLUDE_DIRS})
target_link_libraries(ParticleFilter INTERFACE ${HDF5_CXX_LIBRARIES})
target_link_libraries(ParticleFilter INTERFACE stationsim_project_options stationsim_project_warnings)
target_link_libraries(ParticleFilter INTERFACE MPI::MPI_CXX)

if (STATIONSIM_ENABLE_OPENMP)
    find_package(OpenMP REQUIRED)
    if (OpenMP_CXX_FOUND)
        target_link_libraries(ParticleFilter INTERFACE OpenMP::OpenMP_CXX)
    endif ()
endif ()