#---------------------------------------------------------------------------//
# Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
# Research Computing Services, University of Cambridge, UK
#
# Distributed under The MIT License (MIT)
# See accompanying file LICENSE
#---------------------------------------------------------------------------//

function(ADD_UNIT_TEST EXE_NAME SRC_NAME)
    message(STATUS "Generating ${EXE_NAME} unit test.")

    add_executable(${EXE_NAME})
    target_sources(${EXE_NAME} PUBLIC ${SRC_NAME})

    target_include_directories(${EXE_NAME} PRIVATE
            ${CMAKE_SOURCE_DIR}/include
            ${CMAKE_SOURCE_DIR}/external/include
            ${CMAKE_BINARY_DIR})
    target_link_libraries(${EXE_NAME} PRIVATE ParticleFilter)
    add_test(NAME ${EXE_NAME} COMMAND ${EXE_NAME})
endfunction()

add_unit_test(unit_test_particle_filter unit_test_particle_filter.cpp)
