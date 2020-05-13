//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include "Timer.hpp"
#include "cxxplot.hpp"
#include "model/Model.hpp"
#include "model/ModelParameters.hpp"
#include "model/ModelPlotting.hpp"
#include "particle_filter/ParticleFilter.hpp"
#include "particle_filter/ParticleFilterDataFeed.hpp"
#include "particle_filter/ParticlesInitialiser.hpp"
#include <memory>

class InitialiseModelParticles : public station_sim::ParticlesInitialiser<station_sim::Model> {

  public:
    station_sim::ModelParameters model_parameters;
    std::shared_ptr<station_sim::Model> base_model;

    InitialiseModelParticles() {
        model_parameters.set_population_total(40);
        model_parameters.set_do_print(false);

        base_model = std::make_shared<station_sim::Model>(station_sim::Model(0, model_parameters));
    }

    [[nodiscard]] std::vector<station_sim::Model> initialise_particles(int number_of_particles) const override {

        std::vector<station_sim::Model> particles;
        for (int i = 0; i < number_of_particles; i++) {
            station_sim::Model particle_model = station_sim::Model(i, model_parameters);
            particle_model.set_state(base_model->get_state());

            particles.emplace_back(particle_model);
        }
        return particles;
    }
};

int main() {
    Chronos::Timer timer("timer1");
    timer.start();

    std::shared_ptr<station_sim::ParticleFilterDataFeed<std::vector<float>>> particle_filter_data_feed =
        std::make_shared<station_sim::ParticleFilterDataFeed<std::vector<float>>>(
            station_sim::ParticleFilterDataFeed<std::vector<float>>());

    std::shared_ptr<InitialiseModelParticles> initialise_model_particles =
        std::make_shared<InitialiseModelParticles>(InitialiseModelParticles());

    std::vector<float> simulation_data = particle_filter_data_feed->get_state();

    station_sim::ModelParameters model_parameters;
    model_parameters.set_population_total(40);
    model_parameters.set_do_print(false);
    station_sim::Model base_model(0, model_parameters);

    station_sim::ParticleFilter<station_sim::Model, std::vector<float>> particle_filter(particle_filter_data_feed,
                                                                                        initialise_model_particles);
    particle_filter.step();

    timer.stop_timer(true);

    station_sim::ParticleFilterStatistics particle_filter_statistics = particle_filter.get_particle_filter_statistics();

    auto weighted_mean_errors = particle_filter_statistics.get_weighted_mean_errors();

    std::vector<float> y_int;
    for (const auto &mean : weighted_mean_errors) {
        y_int.emplace_back(mean);
    }

    cxxplot::Plot<float> plot(y_int);
    plot.set_xlabel("Output index");
    plot.set_ylabel("weighted_mean_errors");
    plot.show();

    return 0;
}