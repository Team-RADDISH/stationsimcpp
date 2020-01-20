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
#include "Point2D.hpp"

#include <vector>
#include <array>
#include <random>

namespace station_sim {

	class ModelParameters;
	class Agent;

	class STATIONSIM_EXPORT Model {
	private:
		int model_id;
		int status;
		float speed_step;

		// History related variables
		int history_collisions_number;
		std::vector<Point2D> history_collision_locations;
		int wiggle_collisions_number;
		std::vector<Point2D> history_wiggle_locations;
		int steps_taken;
		int steps_expected;
		int steps_delay;
		std::mt19937* generator;

	public:
		int step_id = 0;
		int pop_active = 0;
		int pop_finished = 0;
		std::array<Point2D, 2> boundaries;
		std::vector<Point2D> gates_in_locations;
		std::vector<Point2D> gates_out_locations;
		std::vector<Agent> agents;
		[[nodiscard]] std::mt19937* get_generator() const;

		Model() = delete;
		Model(int unique_id, const ModelParameters& model_parameters);
		~Model();

		[[nodiscard]] int get_unique_id() const;
		[[nodiscard]] const std::vector<Point2D>& get_gates_in_locations() const;
		[[nodiscard]] const std::vector<Point2D>& get_gates_out_locations() const;
		void step(Model& model, const ModelParameters& model_parameters);
		[[nodiscard]] int get_history_collisions_number() const;
		void set_history_collisions_number(int history_collisions_number);
		void increase_history_collisions_number_by_value(int value_increase);
		void add_to_history_collision_locations(Point2D new_location);
		void increase_wiggle_collisions_number_by_value(int value_increase);
		void add_to_history_wiggle_locations(Point2D new_location);
		[[nodiscard]] float get_speed_step() const;

	private:
		void initialize_model(int unique_id, const ModelParameters& model_parameters);
		void set_boundaries(const ModelParameters& model_parameters);
		void set_gates_locations(const ModelParameters& model_parameters);
		static void create_gates(std::vector<Point2D>& gates, float x, float y, int gates_number);
		void generate_agents(const ModelParameters& model_parameters);
		void move_agents(Model& model, const ModelParameters& model_parameters);
	};
}

#endif //STATIONSIM_MODEL_HPP
