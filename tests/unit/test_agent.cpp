//
// Created by ea461 on 25/11/2019.
//

#define CATCH_CONFIG_MAIN

#include <iostream>

#include "catch.hpp"

#include "Model.hpp"
#include "ModelParameters.hpp"
#include "Agent.hpp"

using namespace station_sim;

TEST_CASE("Test Agent")
{
	ModelParameters model_parameters;
	Model model(0, model_parameters);
	Agent agent(0, model, model_parameters);

	SECTION("Test generated locations") {
//		std::cout << agent.get_agent_location()[0] << std::endl;

//		std::cout << agent.get_agent_speed() << std::endl;

		REQUIRE(5==5);
	}
}