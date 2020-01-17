//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include "HelpFunctions.hpp"

namespace station_sim {

	std::vector<float> HelpFunctions::evenly_spaced_values_within_interval(float start, float stop, float step)
	{
		std::vector<float> result;
		float new_value = start;

		if (start<stop) {
			if (start+step<start) {
				return std::vector<float>();
			}

			while (new_value<stop) {
				result.push_back(new_value);
				new_value += step;
			}
		}
		else {
			if (start+step>start) {
				return std::vector<float>();
			}

			while (new_value>stop) {
				result.push_back(new_value);
				new_value += step;
			}
		}

		return result;
	}

	std::vector<float> HelpFunctions::linear_spaced_vector(float start, float end, int points_number)
	{
		if (points_number<=0) {
			return std::vector<float>();
		}
		
		std::vector<float> result;
		float delta = (end-start)/float(points_number-1);
		for (int i = 0; i<points_number; i++) {
			result.push_back(start+i*delta);
		}
		return result;
	}
}

