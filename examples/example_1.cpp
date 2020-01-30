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
#include "Timer.hpp"

int main()
{
	station_sim::ModelParameters model_parameters;
	model_parameters.set_population_total(100);
	station_sim::Model model(0, model_parameters);

//	station_sim::ModelPlotting::plot_gates_locations(model.gates_in_locations);
//	station_sim::ModelPlotting::plot_gates_locations(model.gates_out_locations);

	Chronos::Timer timer("timer1");
	timer.start();

	for (int i = 0; i<model_parameters.get_step_limit(); i++) {
		model.step(model_parameters);

		if (i%300==0) {
			station_sim::ModelPlotting::plot_agents_locations(model);
		}
	}

	timer.stop_timer(true);

	model.calculate_print_model_run_analytics();

	station_sim::ModelPlotting::plot_agents_trails(model);
}