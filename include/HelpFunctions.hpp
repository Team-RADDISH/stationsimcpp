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
        static std::vector<float> evenly_spaced_values_within_interval(float start, float stop, float step) {
            std::vector<float> result;
            float new_value = start;

            if (start < stop) {
                if (start + step < start) {
                    return std::vector<float>();
                }

                while (new_value < stop) {
                    result.push_back(new_value);
                    new_value += step;
                }
            } else {
                if (start + step > start) {
                    return std::vector<float>();
                }

                while (new_value > stop) {
                    result.push_back(new_value);
                    new_value += step;
                }
            }

            return result;
        }

        static std::vector<float> linear_spaced_vector(float start, float end, int points_number) {
            if (points_number <= 0) {
                return std::vector<float>();
            }

            std::vector<float> result;
            float delta = (end - start) / float(points_number - 1);
            for (int i = 0; i < points_number; i++) {
                result.push_back(start + i * delta);
            }
            return result;
        }
    };
} // namespace station_sim

#endif // STATIONSIM_HELPFUNCTIONS_HPP
