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
#include "particle_filter/ParticleFilter.hpp"
#include "particle_filter/ParticleFilterDataFeed.hpp"
#include "particle_filter/ParticleFilterParameters.hpp"
#include <memory>

int main() {
    Chronos::Timer timer("timer1");
    timer.start();

    std::shared_ptr<station_sim::ParticleFilterDataFeed<std::vector<float>>> particle_filter_data_feed =
        std::make_shared<station_sim::ParticleFilterDataFeed<std::vector<float>>>(
            station_sim::ParticleFilterDataFeed<std::vector<float>>());

    std::vector<float> simulation_data = particle_filter_data_feed->get_state();

    station_sim::ModelParameters model_parameters;
    model_parameters.set_population_total(40);
    model_parameters.set_do_print(false);
    station_sim::Model base_model(0, model_parameters);

    station_sim::ParticleFilter<station_sim::Model, std::vector<float>> particle_filter(base_model,
                                                                                        particle_filter_data_feed);
    particle_filter.step();

    timer.stop_timer(true);

    return 0;
}