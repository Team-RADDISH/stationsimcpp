//---------------------------------------------------------------------------//
// Copyright (c) 2020 Mose Giordano <m.giordano@ucl.ac.uk>
// Research IT Services, UCL, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#define CATCH_CONFIG_MAIN

#include <iostream>

#include "catch.hpp"
#include "Point2D.hpp"

using namespace station_sim;

TEST_CASE("Test Point2D") {

    SECTION("Distances") {
        Point2D p = Point2D(1, 2);
        Point2D projection;

        // Euclidean distance between two points.
		REQUIRE(p.distance(Point2D(1, 2)) == Approx(0.0));
		REQUIRE(p.distance(Point2D(3e38f, 2)) == Approx(3e38f));
		REQUIRE(p.distance(Point2D(6, 14)) == Approx(13));

        // Euclidean distance between a point and a segment.
        // Segment is a point.
		REQUIRE(p.distance_projection(Point2D(6, 14), Point2D(6, 14)).first == Approx(13));
        // Projection would fall outside of the segment, so closest point is the
        // closest vertex of the segment.
		REQUIRE(p.distance_projection(Point2D(6, 14), Point2D(16, 14)).first == Approx(13));
        projection = p.distance_projection(Point2D(6, 14), Point2D(16, 14)).second;
		REQUIRE(projection.x == 6);
        REQUIRE(projection.y == 14);
        // Projection is within the segment.
		REQUIRE(p.distance_projection(Point2D(3, 6), Point2D(6, 3)).first == Approx(4.242640687119285));
        projection = p.distance_projection(Point2D(3, 6), Point2D(6, 3)).second;
		REQUIRE(projection.x == 4);
        REQUIRE(projection.y == 5);
    }
}
