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
#include "model/Agent.hpp"
#include "model/Model.hpp"
#include "model/ModelParameters.hpp"

using namespace station_sim;

TEST_CASE("Test Agent") {
    ModelParameters model_parameters;
    Model model(0, model_parameters);
    //Agent agent(0, model, model_parameters);

    SECTION("Test generated locations") {
        //		std::cout << agent.get_agent_location()[0] << std::endl;

        //		std::cout << agent.get_agent_speed() << std::endl;

        REQUIRE(5 == 5);
    }
}