//
// Created by ea461 on 25/11/2019.
//

#ifndef STATIONSIM_MODEL_HPP
#define STATIONSIM_MODEL_HPP

#include "stationsim_export.h"
#include "ModelParameters.hpp"
#include "Agent.hpp"

#include <vector>

namespace station_sim {
	class STATIONSIM_EXPORT Model {
	private:

	public:
		int unique_id;
		int status;

		float speed_step;
		float boundaries[2][2];

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

	private:
		void initialize_model(int unique_id, const ModelParameters& model_parameters);
		void set_boundaries(const ModelParameters& model_parameters);
		void set_gates_locations(const ModelParameters& model_parameters);
		void create_gates(std::vector<std::vector<double>>& gates, float x, float y, int gates_number);
		std::vector<double> linear_spaced_vector(double start, double end, int points_number);
		void generate_agents(const ModelParameters& model_parameters);
	};
}

#endif //STATIONSIM_MODEL_HPP
