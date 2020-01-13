//
// Created by ea461 on 25/11/2019.
//

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