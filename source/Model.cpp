//
// Created by ea461 on 25/11/2019.
//

#include <iostream>

#include "Model.hpp"

namespace station_sim {

	Model::~Model() = default;

	Model::Model(int unique_id, const ModelParameters& model_parameters)
	{
		initialize_model(unique_id, model_parameters);
	}

	void Model::initialize_model(int unique_id, const ModelParameters& model_parameters)
	{
		model_id = unique_id;
		status = 1;
		step_id = 0;
		pop_active = 0;
		pop_finished = 0;
		history_collisions_number = 0;
		wiggle_collisions_number = 0;

		speed_step =
				(model_parameters.get_speed_mean()-model_parameters.get_speed_min())/model_parameters.get_speed_steps();

		set_boundaries(model_parameters);
		set_gates_locations(model_parameters);

		generate_agents(model_parameters);
	}

	void Model::set_boundaries(const ModelParameters& model_parameters)
	{
		boundaries[0][0] = 0;
		boundaries[0][1] = 0;
		boundaries[1][0] = model_parameters.get_space_width();
		boundaries[1][1] = model_parameters.get_space_height();
	}

	void Model::set_gates_locations(const ModelParameters& model_parameters)
	{
		gates_in_locations.resize(model_parameters.get_gates_in());
		gates_out_locations.resize(model_parameters.get_gates_out());

		create_gates(gates_in_locations, 0, model_parameters.get_space_height(), model_parameters.get_gates_in()+2);
		create_gates(gates_out_locations, model_parameters.get_space_width(), model_parameters.get_space_height(),
				model_parameters.get_gates_out()+2);
	}

	void Model::create_gates(std::vector<std::vector<float>>& gates, float x, float y, int gates_number)
	{
		std::vector<float> result = linear_spaced_vector(0, y, gates_number);
		result.erase(result.begin());
		result.pop_back();

		for (unsigned long i = 0; i<result.size(); i++) {
			gates[i].push_back(x);
			gates[i].push_back(result[i]);
		}
	}

	std::vector<float> Model::linear_spaced_vector(float start, float end, int points_number)
	{
		std::vector<float> result;
		float delta = (end-start)/float(points_number-1);
		for (int i = 0; i<points_number; i++) {
			result.push_back(start+i*delta);
		}
		return result;
	}

	void Model::generate_agents(const ModelParameters& model_parameters)
	{
		for (int i = 0; i<model_parameters.get_population_total(); i++) {
			agents.emplace_back(Agent(i, *this, model_parameters));
		}
	}

	const std::vector<std::vector<float>>& Model::get_gates_in_locations() const
	{
		return gates_in_locations;
	}

	const std::vector<std::vector<float>>& Model::get_gates_out_locations() const
	{
		return gates_out_locations;
	}

	void Model::step(Model& model, const ModelParameters& model_parameters)
	{
		if (pop_finished<model_parameters.get_population_total() && step_id<model_parameters.get_step_limit()
				&& status==1) {
			if (model_parameters.is_do_print() && step_id%100==0) {
				std::cout << "\tIteration: " << (float) (step_id)/(float) (model_parameters.get_step_limit())
						  << std::endl;
			}

			// get agents and move them
			move_agents(model, model_parameters);

			// do the history for the state


			step_id += 1;
		}
		else {
			if (model_parameters.is_do_print() && status==1) {
				std::cout << "StationSim " << model_id << " - Everyone made it!" << std::endl;
			}
		}
	}

	int Model::get_history_collisions_number() const
	{
		return history_collisions_number;
	}
	void Model::set_history_collisions_number(int history_collisions_number)
	{
		this->history_collisions_number = history_collisions_number;
	}

	void Model::increase_history_collisions_number_by_value(int value_increase)
	{
		history_collisions_number += value_increase;
	}

	void Model::add_to_history_collision_locations(std::vector<float> new_location)
	{
		history_collision_locations.push_back(new_location);
	}

	void Model::increase_wiggle_collisions_number_by_value(int value_increase)
	{
		wiggle_collisions_number += value_increase;
	}

	void Model::add_to_history_wiggle_locations(std::vector<float> new_location)
	{
		history_wiggle_locations.push_back(new_location);
	}

	void Model::move_agents(Model& model, const ModelParameters& model_parameters)
	{
		for (auto& agent:agents) {
			agent.move_agent(model, model_parameters);
		}
	}

	int Model::get_unique_id() const
	{
		return model_id;
	}

	float Model::get_speed_step() const
	{
		return speed_step;
	}
}