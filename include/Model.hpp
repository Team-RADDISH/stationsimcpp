//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_MODEL_HPP
#define STATIONSIM_MODEL_HPP

#include "stationsim_export.h"
#include "ModelParameters.hpp"
#include "Agent.hpp"

#include <vector>
#include <array>

namespace station_sim {
	class STATIONSIM_EXPORT Model {
	private:
		int model_id;
		int status;
		float speed_step;

		// History related variables
		int history_collisions_number;
		std::vector<std::array<float, 2>> history_collision_locations;
		int wiggle_collisions_number;
		std::vector<std::array<float, 2>> history_wiggle_locations;
		int steps_taken;
		int steps_expected;
		int steps_delay;

	public:
		int step_id = 0;
		int pop_active = 0;
		int pop_finished = 0;
		std::array<std::array<float, 2>, 2> boundaries;
		std::vector<std::array<float, 2>> gates_in_locations;
		std::vector<std::array<float, 2>> gates_out_locations;
		std::vector<Agent> agents;

		Model() = delete;
		Model(int unique_id, const ModelParameters& model_parameters);
		~Model();

		[[nodiscard]] int get_unique_id() const;
		[[nodiscard]] const std::vector<std::array<float, 2>>& get_gates_in_locations() const;
		[[nodiscard]] const std::vector<std::array<float, 2>>& get_gates_out_locations() const;
		void step(Model& model, const ModelParameters& model_parameters);
		[[nodiscard]] int get_history_collisions_number() const;
		void set_history_collisions_number(int history_collisions_number);
		void increase_history_collisions_number_by_value(int value_increase);
		void add_to_history_collision_locations(std::array<float, 2> new_location);
		void increase_wiggle_collisions_number_by_value(int value_increase);
		void add_to_history_wiggle_locations(std::array<float, 2> new_location);
		[[nodiscard]] float get_speed_step() const;

	private:
		void initialize_model(int unique_id, const ModelParameters& model_parameters);
		void set_boundaries(const ModelParameters& model_parameters);
		void set_gates_locations(const ModelParameters& model_parameters);
		void create_gates(std::vector<std::array<float, 2>>& gates, float x, float y, int gates_number);
		std::vector<float> linear_spaced_vector(float start, float end, int points_number);
		void generate_agents(const ModelParameters& model_parameters);
		void move_agents(Model& model, const ModelParameters& model_parameters);
	};
}

#endif //STATIONSIM_MODEL_HPP
