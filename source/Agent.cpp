//
// Created by ea461 on 07/01/2020.
//

#include "Agent.hpp"
#include "Model.hpp"
#include "help_functions.hpp"

#include <random>
#include <algorithm>

namespace station_sim {

	Agent::Agent(int unique_id, const Model& model, const ModelParameters& model_parameters)
	{
		generator = std::default_random_engine(model_parameters.get_random_seed());
		initialize_random_distributions(model_parameters);

		agent_id = unique_id;
		is_active = false;

		initialize_location(model, model_parameters);
		initialize_speed(model, model_parameters);

//		steps_activate = gates_speed_exponential_distribution(generator);
//		wiggle = fmin(model_parameters.get_max_wiggle(), agent_max_speed);
	}

	Agent::~Agent()
	{

	}

	void Agent::initialize_random_distributions(const ModelParameters& model_parameters)
	{
		float_distribution = std::uniform_real_distribution<float>(-1, 1);
		gates_in_int_distribution = std::uniform_int_distribution<int>(0, model_parameters.get_gates_in()-1);
		gates_out_int_distribution = std::uniform_int_distribution<int>(0, model_parameters.get_gates_out()-1);
		gates_speed_exponential_distribution = std::exponential_distribution<float>(model_parameters.get_gates_speed());
		speed_normal_distribution = std::normal_distribution<float>(model_parameters.get_speed_mean(),
				model_parameters.get_speed_std());
	}

	void Agent::initialize_location(const Model& model, const ModelParameters& model_parameters)
	{
		float perturb = float_distribution(generator)*model_parameters.get_gates_space();

		gate_in = gates_in_int_distribution(generator);

		location_start = model.get_gates_in_locations()[gate_in];
		location_start[1] += perturb;

		gate_out = gates_out_int_distribution(generator);
		location_desire = model.get_gates_out_locations()[gate_out];

		agent_location = location_start;
	}

	void Agent::initialize_speed(const Model& model, const ModelParameters& model_parameters)
	{
		agent_speed = 0;
		agent_max_speed = 0;

		while (agent_max_speed<=model_parameters.get_speed_min()) {
			agent_max_speed = speed_normal_distribution(generator);
		}

		agent_available_speeds = evenly_spaced_values_within_interval(agent_max_speed,
				model_parameters.get_speed_min(), -model.get_speed_step());
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
		std::vector<float> direction = calculate_agent_direction();
		std::vector<float> new_agent_location(2);
		float new_speed = 0;

		for (const auto& speed : agent_available_speeds) {
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
			if (speed==agent_available_speeds.back()) {
				new_agent_location[0] = agent_location[0];

				auto dis = std::uniform_real_distribution<float>(-1, 2);
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
	Agent::clip_vector_values_to_boundaries(std::vector<float>& vec, std::array<std::array<float, 2>, 2> boundaries)
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

	std::vector<float> Agent::calculate_agent_direction()
	{
		float distance = calculate_distance(location_desire, agent_location);

		std::vector<float> direction(2);
		direction[0] = (location_desire[0]-agent_location[0])/distance;
		direction[1] = (location_desire[0]-agent_location[0])/distance;

		return direction;
	}

	float Agent::calculate_distance(std::vector<float> location_0, std::vector<float> location_1)
	{
		float sum = 0;
		for (unsigned long i = 0; i<location_0.size(); i++) {
			sum = powf(location_0[i]+location_1[i], 2);
		}
		return sqrt(sum);
	}

	bool Agent::is_outside_boundaries(const Model& model, const std::vector<float>& location)
	{
		return model.boundaries[0][0]<location[0] && model.boundaries[1][0]>location[0] &&
				model.boundaries[0][1]<location[1] && model.boundaries[1][1]>location[1];
	}

	bool Agent::collides_other_agent(const Model& model, const ModelParameters& model_parameters,
			const std::vector<float>& location)
	{
		for (const auto& agent : model.agents) {
			if (agent.agent_id!=agent_id && agent.is_active
					&& calculate_distance(agent.get_agent_location(), location)>model_parameters.get_separation()) {
				return true;
			}
		}

		return false;
	}

	void Agent::deactivate_agent(Model& model)
	{

	}

	const std::vector<float>& Agent::get_agent_location() const
	{
		return agent_location;
	}

	float Agent::get_agent_speed() const
	{
		return agent_speed;
	}
}