//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include "Point2D.hpp"

namespace station_sim {

    Point2D::Point2D() {
        x = 0;
        y = 0;
    }

    Point2D::Point2D(float x, float y) {
        this->x = x;
        this->y = y;
    }

    Point2D Point2D::operator()(float x, float y) { return Point2D(x, y); }

} // namespace station_sim
