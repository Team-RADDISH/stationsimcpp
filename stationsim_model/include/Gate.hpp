//---------------------------------------------------------------------------//
// Copyright (c) 2020 Mose Giordano <m.giordano@ucl.ac.uk>
// Research IT Services, UCL, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_GATE_HPP
#define STATIONSIM_GATE_HPP

#include <utility>
#include "Point2D.hpp"

namespace station_sim {
    // Class to represent a gate
    class Gate {
      public:
        // The only member of the class is the point which represents the
        // position of the gate.
        Point2D position;

        Gate(Point2D p);
    };

} // namespace station_sim

#endif // STATIONSIM_GATE_HPP
