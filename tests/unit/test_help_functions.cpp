#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "help_functions.hpp"

using namespace station_sim;

TEST_CASE("Test help functions")
{
	SECTION("Test evenly_spaced_values_within_interval") {
		std::vector<float> result_1 = evenly_spaced_values_within_interval(0, 3, 1);
		REQUIRE(result_1[0]==0);
		REQUIRE(result_1[1]==1);
		REQUIRE(result_1[2]==2);

		std::vector<float> result_2 = evenly_spaced_values_within_interval(3, 7, 2);
		REQUIRE(result_2[0]==3);
		REQUIRE(result_2[1]==5);

		std::vector<float> result_3 = evenly_spaced_values_within_interval(3, 1, 10);
		REQUIRE(result_3.size()==0);

		std::vector<float> result_4 = evenly_spaced_values_within_interval(3, 7, -2);
		REQUIRE(result_4.size()==0);

		std::vector<float> result_5 = evenly_spaced_values_within_interval(0.8394, 0.2, -0.2666);
		REQUIRE(result_5[0]==Approx(0.8394));
		REQUIRE(result_5[1]==Approx(0.5728f));
		REQUIRE(result_5[2]==Approx(0.3062f));
	}
}