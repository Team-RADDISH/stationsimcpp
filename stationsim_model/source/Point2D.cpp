//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include <algorithm>
#include <cmath>
#include <utility>
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

    // Euclidean distance between two points
    float Point2D::distance(Point2D p) const {
        return hypot(this->x - p.x, this->y - p.y);
    }

    // Euclidean distance between the given and the segment s1-s2.  Based on
    // https://stackoverflow.com/a/1501725/2442087
    std::pair<float,Point2D> Point2D::distance_projection(Point2D s1, Point2D s2) const {
        // Length of segment s1-s2
        const float l = s2.distance(s1);

        // Fast-track case where s1 == s2
        if (l == 0.0) {
            return std::pair<float,Point2D>(this->distance(s1), s1);
        }

        // Consider the line extending the segment, parameterized as s1 + t (s2 - s1).
        // We find projection of point p onto the line.
        // It falls where t = [(p - s1) . (s2 - s1)] / |s2 - s1|^2
        // We clamp t from [0,1] to handle points outside the segment vw.
        const float t = std::max(0.0e0, std::min(1.0e0, ((this->x - s1.x) * (s2.x - s1.x) + (this->y - s1.y) * (s2.y - s1.y)) / pow(l, 2)));
        // Projection falls on the segment
        const Point2D projection = Point2D(s1.x + t * (s2.x - s1.x), s1.y + t * (s2.y - s1.y));
        // Return distance and projection, we need both
        return std::pair<float,Point2D>(this->distance(projection), projection);
    }


} // namespace station_sim
