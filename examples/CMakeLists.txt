#---------------------------------------------------------------------------//
# Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
# Research Computing Services, University of Cambridge, UK
#
# Distributed under The MIT License (MIT)
# See accompanying file LICENSE
#---------------------------------------------------------------------------//

function(ADD_EXAMPLE EXE_NAME SRC_NAME)
    message(STATUS "Generating ${EXE_NAME} target")

    add_executable(${EXE_NAME})

    if (CMAKE_CXX_COMPILER_ID STREQUAL "PGI")
        if (STATIONSIM_ENABLE_OPENACC)
            message(STATUS "Using OpenACC")
            set(OPENACC_OPTIONS "-acc=verystrict -ta=tesla:managed -Minfo=accel")

            target_compile_options(${EXE_NAME} PUBLIC "$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>")
            target_compile_options(${EXE_NAME} PUBLIC "$<$<CONFIG:RELEASE>:${MY_RELEASE_OPTIONS}>")
        endif ()
    endif ()

    target_sources(${EXE_NAME} PRIVATE ${SRC_NAME})
    target_include_directories(${EXE_NAME} PRIVATE PRIVATE
            ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_BINARY_DIR})
    target_link_libraries(${EXE_NAME} PRIVATE StationSim Chronos)

    set_target_properties(${EXE_NAME} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/examples")

endfunction()

add_example(single_model_run single_model_run.cpp)
add_example(multiple_models_run multiple_models_run.cpp)
add_example(particle_filter_run particle_filter_run.cpp)

if (STATIONSIM_ENABLE_MPI)
    add_example(multiple_models_run_mpi multiple_models_run_mpi.cpp)
endif ()
