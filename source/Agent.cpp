//
// Created by ea461 on 07/01/2020.
//

#include "Agent.hpp"
#include "Model.hpp"

#include <random>
#include <algorithm>

namespace station_sim {

	Agent::Agent(int unique_id, const Model& model, const ModelParameters& model_parameters)
	{
		generator = std::default_random_engine(model_parameters.get_random_seed());

		this->unique_id = unique_id;
		is_active = false;

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

		agent_location = location_start;
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

	void Agent::step(Model& model, const ModelParameters& model_parameters)
	{
		if (is_active==0) {
			activate_agent(model);
		}
		else if (is_active==1) {
			move_agent(model, model_parameters);
			deactivate_agent(model);
		}
	}

	void Agent::activate_agent(Model& model)
	{
		if (model.step_id>steps_activate) {
			is_active = true;
			model.pop_active += 1;
			step_start = model.step_id;
		}
	}

	void Agent::move_agent(Model& model, const ModelParameters& model_parameters)
	{
		std::vector<double> direction = calculate_agent_direction();
		std::vector<double> new_agent_location(2);
		double new_speed = 0;

		for (const auto& speed : speeds) {
			new_speed = speed;
			new_agent_location[0] = agent_location[0]+speed*direction[0];
			new_agent_location[1] = agent_location[1]+speed*direction[1];

			if (is_outside_boundaries(model, new_agent_location)
					|| collides_other_agent(model, model_parameters, new_agent_location)) {
				if (model_parameters.is_do_history()) {
					model.increase_history_collisions_number_by_value(1);
					model.add_to_history_collision_locations(new_agent_location);
				}
			}
			else {
				break;
			}

			// If even the slowest speed results in a collision, then wiggle.
			if (speed==speeds.back()) {
				new_agent_location[0] = agent_location[0];

				auto dis = std::uniform_real_distribution<double>(-1, 2);
				new_agent_location[1] = agent_location[1]+dis(generator);

				if (model_parameters.is_do_history()) {
					model.increase_wiggle_collisions_number_by_value(1);
					model.add_to_history_wiggle_locations(new_agent_location);
				}
			}
		}

		if (is_outside_boundaries(model, new_agent_location)) {
			clip_vector_values_to_boundaries(new_agent_location, model.boundaries);
		}

		agent_location = new_agent_location;
		agent_speed = new_speed;
	}

	void
	Agent::clip_vector_values_to_boundaries(std::vector<double>& vec, std::array<std::array<double, 2>, 2> boundaries)
	{
		if (vec[0]<boundaries[0][0]) {
			vec[0] = boundaries[0][0];
		}
		if (vec[1]<boundaries[1][0]) {
			vec[1] = boundaries[1][0];
		}

		if (vec[0]>boundaries[1][0]) {
			vec[0] = boundaries[1][0];
		}
		if (vec[1]>boundaries[1][1]) {
			vec[1] = boundaries[1][1];
		}
	}

	std::vector<double> Agent::calculate_agent_direction()
	{
		double distance = calculate_distance(location_desire, agent_location);

		std::vector<double> direction(2);
		direction[0] = (location_desire[0]-agent_location[0])/distance;
		direction[1] = (location_desire[0]-agent_location[0])/distance;

		return direction;
	}

	double Agent::calculate_distance(std::vector<double> location_0, std::vector<double> location_1)
	{
		double sum = 0;
		for (unsigned long i = 0; i<location_0.size(); i++) {
			sum = powf(location_0[i]+location_1[i], 2);
		}
		return sqrt(sum);
	}

	bool Agent::is_outside_boundaries(const Model& model, const std::vector<double>& location)
	{
		return model.boundaries[0][0]<location[0] && model.boundaries[1][0]>location[0] &&
				model.boundaries[0][1]<location[1] && model.boundaries[1][1]>location[1];
	}

	bool Agent::collides_other_agent(const Model& model, const ModelParameters& model_parameters,
			const std::vector<double>& location)
	{
		for (const auto& agent : model.agents) {
			if (agent.unique_id!=unique_id && agent.is_active
					&& calculate_distance(agent.get_agent_location(), location)>model_parameters.get_separation()) {
				return true;
			}
		}

		return false;
	}

	void Agent::deactivate_agent(Model& model)
	{

	}

	const std::vector<double>& Agent::get_agent_location() const
	{
		return agent_location;
	}
}