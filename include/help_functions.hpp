//
// Created by ea461 on 13/01/2020.
//

#ifndef STATIONSIM_HELP_FUNCTIONS_HPP
#define STATIONSIM_HELP_FUNCTIONS_HPP

#include <vector>

namespace station_sim {
	// @todo write tests, check if is the same as numpy.arange, (3,1,10) input is different to numpy version
	std::vector<float> evenly_spaced_values_within_interval(float start, float stop, float step)
	{
		std::vector<float> result;
		float new_value = start;

		if (start<stop) {
			while (new_value<stop) {
				result.push_back(new_value);
				new_value += step;
			}
		}
		else {
			while (new_value>stop) {
				result.push_back(new_value);
				new_value = start+step;
				result.push_back(new_value);
			}
		}

		return result;
	}
}

#endif //STATIONSIM_HELP_FUNCTIONS_HPP
