project(unit_tests)

add_executable(test_agent test_agent.cpp)
target_include_directories(test_agent PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/external/include
        ${CMAKE_BINARY_DIR})
target_link_libraries(test_agent PRIVATE StationSim)
add_test(NAME test_agent COMMAND test_agent)

add_executable(test_model test_model.cpp)
target_include_directories(test_model PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/external/include
        ${CMAKE_BINARY_DIR})
target_link_libraries(test_model PRIVATE StationSim)
add_test(NAME test_model COMMAND test_model)

add_executable(test_help_functions test_help_functions.cpp)
target_include_directories(test_help_functions PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/external/include
        ${CMAKE_BINARY_DIR})
target_link_libraries(test_help_functions PRIVATE StationSim)
add_test(NAME test_help_functions COMMAND test_help_functions)