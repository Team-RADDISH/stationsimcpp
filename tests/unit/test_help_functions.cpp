//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "HelpFunctions.hpp"

using namespace station_sim;

TEST_CASE("Test help functions")
{
	SECTION("Test evenly_spaced_values_within_interval") {
		std::vector<float> result_1 = HelpFunctions::evenly_spaced_values_within_interval(0, 3, 1);
		REQUIRE(result_1[0]==0);
		REQUIRE(result_1[1]==1);
		REQUIRE(result_1[2]==2);

		std::vector<float> result_2 = HelpFunctions::evenly_spaced_values_within_interval(3, 7, 2);
		REQUIRE(result_2[0]==3);
		REQUIRE(result_2[1]==5);

		std::vector<float> result_3 = HelpFunctions::evenly_spaced_values_within_interval(3, 1, 10);
		REQUIRE(result_3.size()==0);

		std::vector<float> result_4 = HelpFunctions::evenly_spaced_values_within_interval(3, 7, -2);
		REQUIRE(result_4.size()==0);

		std::vector<float> result_5 = HelpFunctions::evenly_spaced_values_within_interval(0.8394, 0.2, -0.2666);
		REQUIRE(result_5[0]==Approx(0.8394));
		REQUIRE(result_5[1]==Approx(0.5728));
		REQUIRE(result_5[2]==Approx(0.3062));
	}

	SECTION("Test linear_spaced_vector") {
		std::vector<float> result_1 = HelpFunctions::linear_spaced_vector(0, 3, 0);
		REQUIRE(result_1.size()==0);

		std::vector<float> result_2 = HelpFunctions::linear_spaced_vector(0, 3, 2);
		REQUIRE(result_2[0]==0);
		REQUIRE(result_2[1]==3);

		std::vector<float> result_3 = HelpFunctions::linear_spaced_vector(0.01, 2.56, 4);
		REQUIRE(result_3[0]==Approx(0.01));
		REQUIRE(result_3[1]==Approx(0.86));
		REQUIRE(result_3[2]==Approx(1.71));
		REQUIRE(result_3[3]==Approx(2.56));

		std::vector<float> result_4 = HelpFunctions::linear_spaced_vector(6, 3, 3);
		REQUIRE(result_4[0]==Approx(6));
		REQUIRE(result_4[1]==Approx(4.5));
		REQUIRE(result_4[2]==Approx(3));
	}
}