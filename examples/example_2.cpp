//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include "MultipleModelsRun.hpp"
#include "Timer.hpp"

int main()
{
	int number_of_models = 1000;

	Chronos::Timer timer("timer1");
	timer.start();

	station_sim::MultipleModelsRun multiple_models_run;

	for (int i = 0; i<number_of_models; i++) {

		multiple_models_run.model_parameters.emplace_back(station_sim::ModelParameters());
		multiple_models_run.model_parameters.back().set_population_total(100);
		multiple_models_run.model_parameters.back().set_do_print(false);

		station_sim::Model model(i, multiple_models_run.model_parameters.back());

		multiple_models_run.models.push_back(model);

	}

	multiple_models_run.run_all_models();

	timer.stop_timer(true);
}