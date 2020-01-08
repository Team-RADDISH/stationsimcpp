//
// Created by ea461 on 07/01/2020.
//

#ifndef STATIONSIM_AGENT_HPP
#define STATIONSIM_AGENT_HPP

#include "stationsim_export.h"
#include "ModelParameters.hpp"

#include <vector>
#include <random>

namespace station_sim {

	class Model;

	class STATIONSIM_EXPORT Agent {
	private:
		std::default_random_engine generator;
		std::uniform_real_distribution<double> double_distribution;
		std::uniform_int_distribution<int> int_distribution;
		std::exponential_distribution<double> exponential_distribution;

		int unique_id;
		bool status;

		int gate_in;
		std::vector<double> location_start;
		int gate_out;
		std::vector<double> location_desire;
		std::vector<double> location;

		double speed_max;
		std::vector<double> speeds;

		double steps_activate;
		double wiggle;

	public:
		Agent(int unique_id, const Model& model, const ModelParameters& model_parameters);
		~Agent();

	private:
		void initialize_location(const Model& model, const ModelParameters& model_parameters);
		void initialize_speed(const Model& model, const ModelParameters& model_parameters);
		std::vector<double> evenly_spaced_values_within_interval(double start, double stop, double step);
	};
}
#endif //STATIONSIM_AGENT_HPP
