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
		std::uniform_real_distribution<double> double_distribution;
		std::uniform_int_distribution<int> int_distribution;
		std::exponential_distribution<double> exponential_distribution;

		int unique_id;
		bool is_active;

		int gate_in;
		std::vector<double> location_start;
		int gate_out;
		std::vector<double> location_desire;
		std::vector<double> agent_location;
	public:
		const std::vector<double>& get_agent_location() const;
	private:

		double speed_max;
		std::vector<double> speeds;
		double agent_speed;

		double steps_activate;
		double wiggle;

		int step_start;

	public:
		Agent(int unique_id, const Model& model, const ModelParameters& model_parameters);
		~Agent();

		void move_agent(Model& model, const ModelParameters& model_parameters);

	private:
		void initialize_location(const Model& model, const ModelParameters& model_parameters);
		void initialize_speed(const Model& model, const ModelParameters& model_parameters);
		std::vector<double> evenly_spaced_values_within_interval(double start, double stop, double step);
		void step(Model& model, const ModelParameters& model_parameters);
		void activate_agent(Model& model);
		void deactivate_agent(Model& model);
		double calculate_distance(std::vector<double> location_0, std::vector<double> location_1);
		std::vector<double> calculate_agent_direction();
		bool is_outside_boundaries(const Model& model, const std::vector<double>& location);
		bool collides_other_agent(const Model& model, const ModelParameters& model_parameters,
				const std::vector<double>& location);
		void
		clip_vector_values_to_boundaries(std::vector<double>& vec, std::array<std::array<double, 2>, 2> boundaries);
	};
}
#endif //STATIONSIM_AGENT_HPP
