cmake_minimum_required(VERSION 3.15)

project(tests)

add_executable(test_model_parameters test_model_parameters.cpp)
target_include_directories(test_model_parameters PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/external/include
        ${CMAKE_BINARY_DIR})
target_link_libraries(test_model_parameters PRIVATE StationSim)
add_test(NAME test_model_parameters COMMAND test_model_parameters)