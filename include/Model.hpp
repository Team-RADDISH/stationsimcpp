//
// Created by ea461 on 25/11/2019.
//

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

		int history_collisions_number;
		std::vector<std::vector<float>> history_collision_locations;

		int wiggle_collisions_number;
		std::vector<std::vector<float>> history_wiggle_locations;

		float speed_step;

	public:
		std::array<std::array<float, 2>, 2> boundaries;

		std::vector<std::vector<float>> gates_in_locations;
		std::vector<std::vector<float>> gates_out_locations;

		int step_id = 0;
		int pop_active = 0;
		int pop_finished = 0;

		std::vector<Agent> agents;

		Model() = delete;
		Model(int unique_id, const ModelParameters& model_parameters);
		~Model();

		[[nodiscard]] int get_unique_id() const;

		[[nodiscard]] const std::vector<std::vector<float>>& get_gates_in_locations() const;
		[[nodiscard]] const std::vector<std::vector<float>>& get_gates_out_locations() const;

		void step(Model& model, const ModelParameters& model_parameters);

		int get_history_collisions_number() const;
		void set_history_collisions_number(int history_collisions_number);
		void increase_history_collisions_number_by_value(int value_increase);
		void add_to_history_collision_locations(std::vector<float> new_location);

		void increase_wiggle_collisions_number_by_value(int value_increase);
		void add_to_history_wiggle_locations(std::vector<float> new_location);

		float get_speed_step() const;

	private:
		void initialize_model(int unique_id, const ModelParameters& model_parameters);
		void set_boundaries(const ModelParameters& model_parameters);
		void set_gates_locations(const ModelParameters& model_parameters);
		void create_gates(std::vector<std::vector<float>>& gates, float x, float y, int gates_number);
		std::vector<float> linear_spaced_vector(float start, float end, int points_number);
		void generate_agents(const ModelParameters& model_parameters);

		void move_agents(Model& model, const ModelParameters& model_parameters);
	};
}

#endif //STATIONSIM_MODEL_HPP
