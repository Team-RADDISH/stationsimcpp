//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include "Timer.hpp"
#include "model/ModelPlotting.hpp"
#include "model/MultipleModelsRun.hpp"

int main()
{
	Chronos::Timer timer_total("Total runtime");
	Chronos::Timer timer_models_initialisation("Models initialisation");
	Chronos::Timer timer_models_run("Models runs");

	int number_of_models = 100;

	timer_total.start();
	timer_models_initialisation.start();
	station_sim::MultipleModelsRun multiple_models_run;

	for (int i = 0; i<number_of_models; i++) {

		station_sim::ModelParameters model_parameters;
		model_parameters.set_population_total(100);
		model_parameters.set_do_print(false);

		station_sim::Model model(i, model_parameters);

		multiple_models_run.add_model_and_model_parameters(model);
	}
	timer_models_initialisation.stop_timer(true);

	timer_models_run.start();
	multiple_models_run.run_all_models();
	timer_models_run.stop_timer(true);
	timer_total.stop_timer(true);

	multiple_models_run.get_model(0).calculate_print_model_run_analytics();

	//station_sim::ModelPlotting::plot_agents_trails(multiple_models_run.get_model(0));

	return 0;
}