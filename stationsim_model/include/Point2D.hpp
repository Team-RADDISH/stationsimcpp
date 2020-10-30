//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_POINT2D_HPP
#define STATIONSIM_POINT2D_HPP

#include <utility>

namespace station_sim {
    class Point2D {
      public:
        float x, y;

        Point2D();

        Point2D(float x, float y);

        Point2D operator()(float x, float y);

        float distance(Point2D p) const;
        std::pair<float,Point2D> distance_projection(Point2D s1, Point2D s2) const;
    };

} // namespace station_sim

#endif // STATIONSIM_POINT2D_HPP
