include(ExternalProject)
ExternalProject_Add(cxxplot
        INSTALL_DIR ${CMAKE_BINARY_DIR}/cxxplot
        GIT_REPOSITORY git@github.com:avramidis/cxxplot.git
        GIT_PROGRESS 1
        STEP_TARGETS build install
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>)