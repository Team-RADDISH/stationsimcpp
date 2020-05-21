//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include "Timer.hpp"
#include "model/Model.hpp"
#include "model/ModelParameters.hpp"
#include "model/ModelPlotting.hpp"
#include <memory>

int main() {
    station_sim::ModelParameters model_parameters;
    model_parameters.set_population_total(500);
    station_sim::Model model(0, model_parameters);

    station_sim::ModelPlotting::plot_gates_locations(model.gates_in_locations);
    station_sim::ModelPlotting::plot_gates_locations(model.gates_out_locations);

    Chronos::Timer timer("timer1");
    timer.start();

    for (int i = 0; i < model_parameters.get_step_limit(); i++) {
        model.step();

        if (i % 300 == 0) {
            station_sim::ModelPlotting::plot_agents_locations(model);
        }

        if (model.model_simulation_finished()) {
            break;
        }
    }

    model.write_model_output_to_hdf_5("model_run.h5");

    timer.stop_timer(true);

    model.calculate_print_model_run_analytics();

    station_sim::ModelPlotting::plot_agents_trails(model);

    return 0;
}