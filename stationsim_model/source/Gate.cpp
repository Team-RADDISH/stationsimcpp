//---------------------------------------------------------------------------//
// Copyright (c) 2020 Mose Giordano <m.giordano@ucl.ac.uk>
// Research IT Services, UCL, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include <utility>
#include "Point2D.hpp"
#include "Gate.hpp"

namespace station_sim {
    Gate::Gate(Point2D p) {
        this->position = p;
    }
} // namespace station_sim
