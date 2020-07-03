//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_POINT2D_HPP
#define STATIONSIM_POINT2D_HPP

namespace station_sim {
    class Point2D {
      public:
        float x, y;

        Point2D();

        Point2D(float x, float y);

        Point2D operator()(float x, float y);
    };

} // namespace station_sim

#endif // STATIONSIM_POINT2D_HPP
