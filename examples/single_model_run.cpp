//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include "Timer.hpp"
#include "Model.hpp"
#include "ModelParameters.hpp"
#include "ModelPlotting.hpp"
#include <memory>

int main() {
    station_sim::ModelParameters model_parameters;
    model_parameters.set_boundaries({ station_sim::Point2D(0, 0), station_sim::Point2D(0, 300), station_sim::Point2D(150, 300), station_sim::Point2D(150, 0) });
    model_parameters.set_gates_in({ station_sim::Gate(station_sim::Point2D(0, 75)), station_sim::Gate(station_sim::Point2D(0, 150)), station_sim::Gate(station_sim::Point2D(0, 225)) });
    model_parameters.set_gates_out({ station_sim::Gate(station_sim::Point2D(150, 100)), station_sim::Gate(station_sim::Point2D(150, 200)) });
    model_parameters.set_population_total(60);
    station_sim::Model model(0, model_parameters);

    station_sim::ModelPlotting::plot_gates_locations(model.get_gates_in());
    station_sim::ModelPlotting::plot_gates_locations(model.get_gates_out());

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
