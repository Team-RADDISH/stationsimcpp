//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_AGENT_HPP
#define STATIONSIM_AGENT_HPP

#include "stationsim_export.h"
#include "ModelParameters.hpp"

#include <vector>
#include <random>
#include <array>

namespace station_sim {

	class Model;

	enum class AgentStatus : int {
		not_started = 0,
		active = 1,
		finished = 2
	};

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
		std::array<float, 2> start_location;
		int gate_out;
		std::array<float, 2> desired_location;
		std::array<float, 2> agent_location;
		float steps_activate;
		float wiggle;
		float agent_max_speed;
		std::vector<float> agent_available_speeds;
		float agent_speed;
		std::vector<std::array<float, 2>> history_locations;
		std::vector<float> history_speeds;
		int history_wiggles = 0;
		int history_collisions = 0;
		int step_start;
		AgentStatus status;

	public:
		Agent() = delete;
		Agent(int unique_id, const Model& model, const ModelParameters& model_parameters);
		~Agent() = default;

		void step(Model& model, const ModelParameters& model_parameters);
		[[nodiscard]] const std::array<float, 2>& get_agent_location() const;
		[[nodiscard]] float get_agent_speed() const;

	private:
		void initialize_location(const Model& model, const ModelParameters& model_parameters);
		void initialize_speed(const Model& model, const ModelParameters& model_parameters);
		void activate_agent(Model& model);
		void deactivate_agent_if_reached_exit_gate(Model& model, const ModelParameters& model_parameters);
		float calculate_distance(std::array<float, 2> location_0, std::array<float, 2> location_1);
		std::vector<float> calculate_agent_direction();
		bool is_outside_boundaries(const Model& model, const std::array<float, 2>& location);
		bool collides_other_agent(const Model& model, const ModelParameters& model_parameters,
				const std::array<float, 2>& location);
		void
		clip_vector_values_to_boundaries(std::array<float, 2>& vec, std::array<std::array<float, 2>, 2> boundaries);
		void initialize_random_distributions(const ModelParameters& model_parameters);

		void move_agent(Model& model, const ModelParameters& model_parameters);
	};
}
#endif //STATIONSIM_AGENT_HPP
