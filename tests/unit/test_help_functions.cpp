#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "help_functions.hpp"

using namespace station_sim;

TEST_CASE("Test help functions")
{
	SECTION("Test evenly_spaced_values_within_interval") {

		REQUIRE(10==10);

		std::vector<float> result = evenly_spaced_values_within_interval(0, 3, 1);

		for (const auto& v:result) {
			std::cout << v << std::endl;
		}
	}
}