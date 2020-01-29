#---------------------------------------------------------------------------//
# Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
# Research Computing Services, University of Cambridge, UK
#
# Distributed under The MIT License (MIT)
# See accompanying file LICENSE
#---------------------------------------------------------------------------//

include(FetchContent)
FetchContent_Declare(Chronos
        GIT_REPOSITORY https://github.com/avramidis/chronos.git
        GIT_TAG origin/master)

FetchContent_MakeAvailable(Chronos)


include(FetchContent)
FetchContent_Declare(cxxplot
        GIT_REPOSITORY https://github.com/avramidis/cxxplot.git
        GIT_TAG origin/master)

FetchContent_MakeAvailable(cxxplot)
