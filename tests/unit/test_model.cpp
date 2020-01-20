//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "ModelParameters.hpp"
#include "Model.hpp"

using namespace station_sim;

TEST_CASE("Test Model")
{
	ModelParameters modelparameters;
	Model model(10, modelparameters);

	SECTION("Test default values") {
		REQUIRE(model.get_unique_id()==10);
		std::cout << model.get_speed_step() << std::endl;
	}
}