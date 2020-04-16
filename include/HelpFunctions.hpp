//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_HELPFUNCTIONS_HPP
#define STATIONSIM_HELPFUNCTIONS_HPP

#include "stationsim_export.h"
#include <vector>

namespace station_sim {
    class STATIONSIM_EXPORT HelpFunctions {
      public:
        static std::vector<float> evenly_spaced_values_within_interval(float start, float stop, float step);

        static std::vector<float> linear_spaced_vector(float start, float end, int points_number);
    };
} // namespace station_sim

#endif // STATIONSIM_HELPFUNCTIONS_HPP
