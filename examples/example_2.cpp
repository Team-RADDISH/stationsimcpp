//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include "MultipleModelsRun.hpp"
#include "Timer.hpp"
#include "ModelPlotting.hpp"

int main()
{
	Chronos::Timer timer_models_initialisation("Models initialisation");
	Chronos::Timer timer_models_run("Models runs");

	int number_of_models = 100;

	timer_models_initialisation.start();
	station_sim::MultipleModelsRun multiple_models_run;

	for (int i = 0; i<number_of_models; i++) {

		multiple_models_run.model_parameters.emplace_back(station_sim::ModelParameters());
		multiple_models_run.model_parameters.back().set_population_total(100);
		multiple_models_run.model_parameters.back().set_do_print(false);

		station_sim::Model model(i, multiple_models_run.model_parameters.back());

		multiple_models_run.models.push_back(model);

	}
	timer_models_initialisation.stop_timer(true);

	timer_models_run.start();
	multiple_models_run.run_all_models();
	timer_models_run.stop_timer(true);

	multiple_models_run.models[0].calculate_print_model_run_analytics();

	station_sim::ModelPlotting::plot_agents_trails(multiple_models_run.models[0]);
}