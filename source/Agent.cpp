//
// Created by ea461 on 07/01/2020.
//

#include "Agent.hpp"
#include "Model.hpp"

#include <random>

namespace station_sim {

	Agent::Agent(int unique_id, const Model& model, const ModelParameters& model_parameters)
	{
		generator = std::default_random_engine(model_parameters.get_random_seed());

		this->unique_id = unique_id;
		status = false;

		initialize_location(model, model_parameters);
		initialize_speed(model, model_parameters);

		exponential_distribution = std::exponential_distribution<double>(model_parameters.get_gates_speed());
		steps_activate = exponential_distribution(generator);
		wiggle = fmin(model_parameters.get_max_wiggle(), speed_max);
	}

	Agent::~Agent()
	{

	}

	void Agent::initialize_location(const Model& model, const ModelParameters& model_parameters)
	{

		double_distribution = std::uniform_real_distribution<double>(-1, 1);
		int_distribution = std::uniform_int_distribution<int>(0, model_parameters.get_gates_in()-1);

		double perturb = double_distribution(generator)*model_parameters.get_gates_space();

		gate_in = int_distribution(generator);

		location_start = model.get_gates_in_locations()[gate_in];
		location_start[1] += perturb;

		gate_out = int_distribution(generator);
		location_desire = model.get_gates_out_locations()[gate_out];

		location = location_start;
	}

	void Agent::initialize_speed(const Model& model, const ModelParameters& model_parameters)
	{
		std::normal_distribution<double> normal_distribution(model_parameters.get_speed_mean(),
				model_parameters.get_speed_std());

		speed_max = 0;

		while (speed_max<=model_parameters.get_speed_min()) {
			speed_max = normal_distribution(generator);
		}

		speeds = evenly_spaced_values_within_interval(speed_max,
				model_parameters.get_speed_min(), -model_parameters.get_speed_steps());
	}

	// @todo write tests, check if is the same as numpy.arange, (3,1,10) input is different to numpy version
	std::vector<double> Agent::evenly_spaced_values_within_interval(double start, double stop, double step)
	{
		std::vector<double> result;
		double new_value = start;

		if (start<stop) {
			while (new_value<stop) {
				result.push_back(new_value);
				new_value = start+step;
				result.push_back(new_value);
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