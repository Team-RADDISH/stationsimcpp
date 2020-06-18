//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_MODELSTATE_HPP
#define STATIONSIM_MODELSTATE_HPP

#include "array"
#include "model/Point2D.hpp"
#include "stationsim_export.h"
#include <algorithm>
#include <vector>

namespace station_sim {

    enum class STATIONSIM_EXPORT AgentActiveStatus : int { not_active = 0, active = 1 };

    class STATIONSIM_EXPORT ModelState {
      public:
        std::vector<Point2D> agents_location;
        std::vector<AgentActiveStatus> agent_active_status;
        std::vector<Point2D> agents_desired_location;

        ModelState() = default;
        ~ModelState() = default;
    };
} // namespace station_sim

#endif // STATIONSIM_MODELSTATE_HPP
