//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#define CATCH_CONFIG_MAIN

#include <iostream>
#include <memory>

#include "catch.hpp"
#include "Agent.hpp"
#include "Model.hpp"
#include "ModelParameters.hpp"

using namespace station_sim;

TEST_CASE("Test Agent") {
    ModelParameters model_parameters;
    Model model(0, model_parameters);
    //Agent agent(0, model, model_parameters);

    SECTION("Test generated locations") {
        std::vector<Point2D> boundaries{ {Point2D(0, 0), Point2D(0, 4), Point2D(4, 4), Point2D(4, 0)} };
        // Square, all points are inside
		REQUIRE(!Agent::is_outside_boundaries(boundaries, Point2D(1, 2)));
		REQUIRE(!Agent::is_outside_boundaries(boundaries, Point2D(2, 1)));
		REQUIRE(!Agent::is_outside_boundaries(boundaries, Point2D(3, 2)));
		REQUIRE(!Agent::is_outside_boundaries(boundaries, Point2D(2, 3)));

        // Hourglass, only the points with y = 2 are inside
        boundaries = { {Point2D(0, 0), Point2D(4, 4), Point2D(0, 4), Point2D(4, 0)} };
		REQUIRE(Agent::is_outside_boundaries(boundaries, Point2D(1, 2)));
		REQUIRE(!Agent::is_outside_boundaries(boundaries, Point2D(2, 1)));
		REQUIRE(Agent::is_outside_boundaries(boundaries, Point2D(3, 2)));
		REQUIRE(!Agent::is_outside_boundaries(boundaries, Point2D(2, 3)));
    }
}
