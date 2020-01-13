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
		int history_collisions_number;
		std::vector<std::vector<double>> history_collision_locations;

		int wiggle_collisions_number;
		std::vector<std::vector<double>> history_wiggle_locations;
	public:
		int unique_id;
		int status;

		float speed_step;
		std::array<std::array<double, 2>, 2> boundaries;

		std::vector<std::vector<double>> gates_in_locations;
		std::vector<std::vector<double>> gates_out_locations;

		int step_id = 0;
		int pop_active = 0;
		int pop_finished = 0;

		std::vector<Agent> agents;

		Model();
		Model(int unique_id);
		Model(const ModelParameters& model_parameters);
		Model(int unique_id, const ModelParameters& model_parameters);
		~Model();

		[[nodiscard]] const std::vector<std::vector<double>>& get_gates_in_locations() const;
		[[nodiscard]] const std::vector<std::vector<double>>& get_gates_out_locations() const;

		void step(Model& model, const ModelParameters& model_parameters);

		int get_history_collisions_number() const;
		void set_history_collisions_number(int history_collisions_number);
		void increase_history_collisions_number_by_value(int value_increase);
		void add_to_history_collision_locations(std::vector<double> new_location);

		void increase_wiggle_collisions_number_by_value(int value_increase);
		void add_to_history_wiggle_locations(std::vector<double> new_location);

	private:
		void initialize_model(int unique_id, const ModelParameters& model_parameters);
		void set_boundaries(const ModelParameters& model_parameters);
		void set_gates_locations(const ModelParameters& model_parameters);
		void create_gates(std::vector<std::vector<double>>& gates, float x, float y, int gates_number);
		std::vector<double> linear_spaced_vector(double start, double end, int points_number);
		void generate_agents(const ModelParameters& model_parameters);

		void move_agents(Model& model, const ModelParameters& model_parameters);
	};
}

#endif //STATIONSIM_MODEL_HPP
