//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include <iostream>
#include <algorithm>

#include "ModelParameters.hpp"
#include "Model.hpp"
#include "Agent.hpp"
#include "HelpFunctions.hpp"

namespace station_sim {

	Model::~Model() = default;

	Model::Model(int unique_id, const ModelParameters& model_parameters)
	{
		initialize_model(unique_id, model_parameters);
		print_per_steps = 100;

		history_state = std::vector<std::vector<Point2D>>(model_parameters.get_step_limit(),
				std::vector<Point2D>(model_parameters.get_population_total()));
	}

	void Model::initialize_model(int unique_id, const ModelParameters& model_parameters)
	{
		generator = new std::mt19937(model_parameters.get_random_seed());

		model_id = unique_id;
		status = ModelStatus::active;
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
		boundaries[0].x = 0; // x1
		boundaries[0].y = 0; // y1
		boundaries[1].x = model_parameters.get_space_width();  // x2
		boundaries[1].y = model_parameters.get_space_height(); // y2
	}

	void Model::set_gates_locations(const ModelParameters& model_parameters)
	{
		gates_in_locations.resize(model_parameters.get_gates_in());
		gates_out_locations.resize(model_parameters.get_gates_out());

		create_gates(gates_in_locations, 0, model_parameters.get_space_height(), model_parameters.get_gates_in()+2);
		create_gates(gates_out_locations, model_parameters.get_space_width(), model_parameters.get_space_height(),
				model_parameters.get_gates_out()+2);
	}

	void Model::create_gates(std::vector<Point2D>& gates, float x, float y, int gates_number)
	{
		std::vector<float> result = HelpFunctions::linear_spaced_vector(0, y, gates_number);
		result.erase(result.begin());
		result.pop_back();

		for (unsigned long i = 0; i<result.size(); i++) {
			gates[i].x = x;
			gates[i].y = result[i];
		}
	}

	void Model::generate_agents(const ModelParameters& model_parameters)
	{
		for (int i = 0; i<model_parameters.get_population_total(); i++) {
			agents.emplace_back(Agent(i, *this, model_parameters));
		}
	}

	const std::vector<Point2D>& Model::get_gates_in_locations() const
	{
		return gates_in_locations;
	}

	const std::vector<Point2D>& Model::get_gates_out_locations() const
	{
		return gates_out_locations;
	}

	void Model::step(Model& model, const ModelParameters& model_parameters)
	{
		if (pop_finished<model_parameters.get_population_total() && step_id<model_parameters.get_step_limit()
				&& status==ModelStatus::active) {
			if (model_parameters.is_do_print() && step_id%print_per_steps==0) {
				std::cout << "\tIteration: " << step_id << "/" << model_parameters.get_step_limit() << std::endl;
			}

			// get agents and move them
			move_agents(model, model_parameters);

			if (model_parameters.is_do_history()) {
				history_state[step_id] = get_agents_location();
			}

			step_id += 1;
		}
		else {
			if (pop_finished<model_parameters.get_population_total()) {
				status = ModelStatus::finished;

				if (model_parameters.is_do_print() && status==ModelStatus::active) {
					std::cout << "StationSim " << model_id << " - Everyone made it!" << std::endl;
				}
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

	void Model::add_to_history_collision_locations(Point2D new_location)
	{
		history_collision_locations.push_back(new_location);
	}

	void Model::increase_wiggle_collisions_number_by_value(int value_increase)
	{
		wiggle_collisions_number += value_increase;
	}

	void Model::add_to_history_wiggle_locations(Point2D new_location)
	{
		history_wiggle_locations.push_back(new_location);
	}

	void Model::move_agents(Model& model, const ModelParameters& model_parameters)
	{
		for (auto& agent:agents) {
			agent.step(model, model_parameters);
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

	std::mt19937* Model::get_generator() const
	{
		return generator;
	}

	std::vector<Point2D> Model::get_agents_location()
	{
		std::vector<Point2D> agents_locations;

		for (const Agent& agent:agents) {
			agents_locations.push_back(agent.get_agent_location());
		}

		return std::vector<Point2D>();
	}

	void Model::calculate_print_model_run_analytics()
	{
		std::cout << "Finish step number: " << step_id << std::endl;
		std::cout << "Population size reached target: " << pop_finished << std::endl;
		std::cout << "Population size active: " << pop_active << std::endl;
		std::cout << "Mean steps taken per agent: "
				  << std::reduce(steps_taken.begin(), steps_taken.end(), 0.0)/agents.size() << std::endl;
		std::cout << "Expected steps per agent: "
				  << std::reduce(steps_expected.begin(), steps_expected.end(), 0.0)/agents.size() << std::endl;
		std::cout << "Mean time delay per agent: "
				  << std::reduce(steps_delay.begin(), steps_delay.end(), 0.0)/agents.size() << std::endl;
		std::cout << "Mean number of collisions per agent: "
				  << std::accumulate(agents.begin(), agents.end(), 0.0, [&](int total, const Agent& agent) {
					return total += agent.get_history_collisions();
				  })/agents.size() << std::endl;
		std::cout << "Mean number of wiggles per agent: "
				  << std::accumulate(agents.begin(), agents.end(), 0.0, [&](int total, const Agent& agent) {
					return total += agent.get_history_wiggles();
				  })/agents.size() << std::endl;

	}
}