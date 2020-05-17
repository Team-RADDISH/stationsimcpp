#---------------------------------------------------------------------------//
# Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
# Research Computing Services, University of Cambridge, UK
#
# Distributed under The MIT License (MIT)
# See accompanying file LICENSE
#---------------------------------------------------------------------------//

add_library(StationSim SHARED)

target_sources(StationSim PRIVATE
        model/Model.cpp
        model/ModelParameters.cpp
        model/MultipleModelsRun.cpp
        model/Agent.cpp
        model/ModelPlotting.cpp
        HelpFunctions.cpp
        model/Point2D.cpp
        particle_filter/ParticleFilterParameters.cpp
        particle_filter/ParticleFilterModelParameters.cpp)
target_include_directories(StationSim PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR})

find_package(HDF5 REQUIRED COMPONENTS CXX)
if (${HDF5_FOUND})
    message(STATUS "HDF5 FOUND!")
    message(STATUS ${HDF5_CXX_INCLUDE_DIRS})
    message(STATUS ${HDF5_CXX_LIBRARIES})
endif ()

target_link_libraries(StationSim PUBLIC Python3::Python Python3::NumPy)
target_include_directories(StationSim PUBLIC ${HDF5_CXX_INCLUDE_DIRS})
target_link_libraries(StationSim PUBLIC ${HDF5_CXX_LIBRARIES})
target_link_libraries(StationSim PUBLIC stationsim_project_options stationsim_project_warnings)

if (STATIONSIM_ENABLE_OPENMP)
    find_package(OpenMP REQUIRED)
    if (OpenMP_CXX_FOUND)
        target_link_libraries(StationSim PUBLIC OpenMP::OpenMP_CXX)
        target_link_libraries(StationSim PUBLIC OpenMP::OpenMP_CXX)
        target_link_libraries(StationSim PUBLIC OpenMP::OpenMP_CXX)
    endif ()
endif ()

if (STATIONSIM_ENABLE_MPI)
    find_package(MPI REQUIRED)
    if (MPI_FOUND)
        message(STATUS "USING MPI")

        target_link_libraries(StationSim PUBLIC MPI::MPI_CXX)
        target_link_libraries(StationSim PUBLIC MPI::MPI_CXX)
        target_link_libraries(StationSim PUBLIC MPI::MPI_CXX)

        target_sources(StationSim PRIVATE
                model/MultipleModelsRunMPI.cpp)
    endif ()
endif ()

target_link_libraries(StationSim PUBLIC cxxplot)
target_link_libraries(StationSim PUBLIC Chronos)

set_target_properties(StationSim PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
        SomeProjConfigVersion.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion)

if (CMAKE_HOST_WIN32)
    target_compile_definitions(StationSim PUBLIC StationSim_EXPORT)

    if (STATIONSIM_ENABLE_EXAMPLES)
        add_custom_command(TARGET StationSim POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy
                "$<TARGET_FILE:StationSim>"
                "${CMAKE_CURRENT_BINARY_DIR}/bin/examples/$<TARGET_FILE_NAME:StationSim>"
                COMMENT "Copying StationSim.dll to examples directory")

        add_custom_command(TARGET StationSim POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy
                "$<TARGET_FILE:cxxplot>"
                "${CMAKE_CURRENT_BINARY_DIR}/bin/examples/$<TARGET_FILE_NAME:cxxplot>"
                COMMENT "Copying cxxplot.dll to examples directory")

        add_custom_command(TARGET StationSim POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy
                "$<TARGET_FILE:Chronos>"
                "${CMAKE_CURRENT_BINARY_DIR}/bin/examples/$<TARGET_FILE_NAME:Chronos>"
                COMMENT "Copying Chronos.dll to examples directory")
    endif ()

    if (STATIONSIM_ENABLE_TESTS)
        add_custom_command(TARGET StationSim POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy
                "$<TARGET_FILE:StationSim>"
                "${CMAKE_CURRENT_BINARY_DIR}/bin/tests/$<TARGET_FILE_NAME:StationSim>"
                COMMENT "Copying StationSim.dll to examples directory")

        add_custom_command(TARGET StationSim POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy
                "$<TARGET_FILE:cxxplot>"
                "${CMAKE_CURRENT_BINARY_DIR}/bin/tests/$<TARGET_FILE_NAME:cxxplot>"
                COMMENT "Copying cxxplot.dll to examples directory")

        add_custom_command(TARGET StationSim POST_BUILD
                COMMAND "${CMAKE_COMMAND}" -E copy
                "$<TARGET_FILE:Chronos>"
                "${CMAKE_CURRENT_BINARY_DIR}/bin/tests/$<TARGET_FILE_NAME:Chronos>"
                COMMENT "Copying Chronos.dll to examples directory")
    endif ()
endif ()