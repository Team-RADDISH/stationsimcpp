//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include "ModelParameters.hpp"
#include "Model.hpp"
#include "ModelPlotting.hpp"

int main()
{
	station_sim::ModelParameters model_parameters;
	station_sim::Model model(0, model_parameters);

//	station_sim::ModelPlotting::plot_gates_locations(model.gates_in_locations);
//	station_sim::ModelPlotting::plot_gates_locations(model.gates_out_locations);

	for (int i = 0; i<model_parameters.get_step_limit(); i++) {
		model.step(model, model_parameters);

	}

	station_sim::ModelPlotting::plot_agents_locations(model);
}