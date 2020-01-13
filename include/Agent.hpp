//
// Created by ea461 on 07/01/2020.
//

#ifndef STATIONSIM_AGENT_HPP
#define STATIONSIM_AGENT_HPP

#include "stationsim_export.h"
#include "ModelParameters.hpp"

#include <vector>
#include <random>
#include <array>

namespace station_sim {

	class Model;

	class STATIONSIM_EXPORT Agent {
	private:
		std::default_random_engine generator;
		std::uniform_real_distribution<float> float_distribution;
		std::uniform_int_distribution<int> gates_in_int_distribution;
		std::uniform_int_distribution<int> gates_out_int_distribution;
		std::exponential_distribution<float> gates_speed_exponential_distribution;
		std::normal_distribution<float> speed_normal_distribution;

		int agent_id;
		bool is_active;

		int gate_in;
		std::vector<float> location_start;
		int gate_out;
		std::vector<float> location_desire;
		std::vector<float> agent_location;
	public:
		const std::vector<float>& get_agent_location() const;
	private:

		float agent_max_speed;
		std::vector<float> agent_available_speeds;
		float agent_speed;
	public:
		float get_agent_speed() const;
	private:

		float steps_activate;
		float wiggle;

		int step_start;

	public:
		Agent(int unique_id, const Model& model, const ModelParameters& model_parameters);
		~Agent();

		void move_agent(Model& model, const ModelParameters& model_parameters);

	private:
		void initialize_location(const Model& model, const ModelParameters& model_parameters);
		void initialize_speed(const Model& model, const ModelParameters& model_parameters);
		void step(Model& model, const ModelParameters& model_parameters);
		void activate_agent(Model& model);
		void deactivate_agent(Model& model);
		float calculate_distance(std::vector<float> location_0, std::vector<float> location_1);
		std::vector<float> calculate_agent_direction();
		bool is_outside_boundaries(const Model& model, const std::vector<float>& location);
		bool collides_other_agent(const Model& model, const ModelParameters& model_parameters,
				const std::vector<float>& location);
		void
		clip_vector_values_to_boundaries(std::vector<float>& vec, std::array<std::array<float, 2>, 2> boundaries);
		void initialize_random_distributions(const ModelParameters& model_parameters);
	};
}
#endif //STATIONSIM_AGENT_HPP
