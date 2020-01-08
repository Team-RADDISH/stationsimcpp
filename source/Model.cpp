//
// Created by ea461 on 25/11/2019.
//

#include "Model.hpp"

namespace station_sim {

	Model::Model()
	{
		initialize_model(0, ModelParameters());
	}

	Model::~Model() = default;

	Model::Model(int unique_id)
	{
		initialize_model(unique_id, ModelParameters());
	}

	Model::Model(const ModelParameters& model_parameters)
	{
		initialize_model(0, model_parameters);
	}

	Model::Model(int unique_id, const ModelParameters& model_parameters)
	{
		initialize_model(unique_id, model_parameters);
	}

	void Model::initialize_model(int unique_id, const ModelParameters& model_parameters)
	{
		status = 1;
		this->unique_id = unique_id;

		speed_step =
				(model_parameters.get_speed_mean()-model_parameters.get_speed_min())/model_parameters.get_speed_steps();

		set_boundaries(model_parameters);
		set_gates_locations(model_parameters);

		step_id = 0;
		pop_active = 0;
		pop_finished = 0;

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

	void Model::create_gates(std::vector<std::vector<double>>& gates, float x, float y, int gates_number)
	{
		std::vector<double> result = linear_spaced_vector(0, y, gates_number);
		result.erase(result.begin());
		result.pop_back();

		for (int i = 0; i<result.size(); i++) {
			gates[i].push_back(x);
			gates[i].push_back(result[i]);
		}
	}

	std::vector<double> Model::linear_spaced_vector(double start, double end, int points_number)
	{
		std::vector<double> result;
		double delta = (end-start)/double(points_number-1);
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

	const std::vector<std::vector<double>>& Model::get_gates_in_locations() const
	{
		return gates_in_locations;
	}

	const std::vector<std::vector<double>>& Model::get_gates_out_locations() const
	{
		return gates_out_locations;
	}
}