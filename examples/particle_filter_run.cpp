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
#include "model/ModelState.hpp"
#include "model/ModelStatistics.hpp"
#include "particle_filter/ParticleFilter.hpp"
#include "particle_filter/ParticleFilterDataFeed.hpp"
#include "particle_filter/ParticleFit.hpp"
#include "particle_filter/ParticlesInitialiser.hpp"
#include <memory>

using namespace station_sim;

class SyntheticDataFeed : public ParticleFilterDataFeed<ModelState> {
  private:
    station_sim::ModelParameters model_parameters;
    station_sim::Model base_model;
    std::shared_ptr<std::mt19937> generator;
    std::normal_distribution<float> float_normal_distribution;

  public:
    SyntheticDataFeed() {
        std::random_device r;
        generator = std::make_shared<std::mt19937>(std::mt19937(r()));
        float target_model_std = 1.0;
        float_normal_distribution = std::normal_distribution<float>(0.0, powf(target_model_std, 2));

        model_parameters = ModelParameters();
        model_parameters.set_population_total(40);
        model_parameters.set_do_print(false);
        base_model = station_sim::Model(0, model_parameters);
    }
    ~SyntheticDataFeed() override = default;

    void progress_feed() override { base_model.step(); }

    [[nodiscard]] ModelState get_state() override {
        ModelState model_state;

        std::vector<Point2D> measured_state(base_model.agents.size());
        std::vector<AgentActiveStatus> agent_active_status(base_model.agents.size());

        // Add noise to the synthetic target data
        for (unsigned long i = 0; i < base_model.agents.size(); i++) {
            Point2D agent_location = base_model.agents[i].get_agent_location();
            measured_state[i].x = agent_location.x + float_normal_distribution(*generator);
            measured_state[i].y = agent_location.y + float_normal_distribution(*generator);

            if (base_model.agents[i].getStatus() == AgentStatus::active) {
                agent_active_status[i] = AgentActiveStatus::active;
            } else {
                agent_active_status[i] = AgentActiveStatus::not_active;
            }
        }
        model_state.agents_location = measured_state;
        model_state.agent_active_status = agent_active_status;

        return model_state;
    }

    void print_statistics() override {
        std::vector<bool> base_model_agents_status = get_base_model_agents_status(base_model);
        long base_model_active_agents = calculate_base_model_active_agents_number(base_model_agents_status);
        //        this->active_agents.push_back(base_model_active_agents);
        std::cout << "Base model active agents: " << base_model_active_agents << std::endl;
    }

    long calculate_base_model_active_agents_number(std::vector<bool> base_model_agents_status) {
        return std::count_if(base_model_agents_status.begin(), base_model_agents_status.end(),
                             [](bool agent_status) { return agent_status; });
    }

    std::vector<bool> get_base_model_agents_status(const Model &base_model) {
        std::vector<bool> base_model_agents_status(base_model.agents.size());
        std::transform(base_model.agents.begin(), base_model.agents.end(), base_model_agents_status.begin(),
                       [](const Agent &agent) -> bool { return agent.getStatus() == AgentStatus::active; });

        return base_model_agents_status;
    }

    [[nodiscard]] const Model &get_base_model() const { return base_model; }
};

class InitialiseModelParticles : public ParticlesInitialiser<Model> {
  public:
    ModelParameters model_parameters;
    std::shared_ptr<Model> base_model;

    InitialiseModelParticles() {
        model_parameters.set_population_total(40);
        model_parameters.set_do_print(false);

        base_model = std::make_shared<Model>(Model(0, model_parameters));
    }

    [[nodiscard]] std::shared_ptr<std::vector<Model>> initialise_particles(int number_of_particles) const override {
        std::shared_ptr<std::vector<Model>> particles =
            std::make_shared<std::vector<Model>>(std::vector<Model>(number_of_particles));
#pragma omp parallel for shared(particles)
        for (int i = 0; i < number_of_particles; i++) {
            (*particles).at(i) = Model(*base_model);
        }
        return particles;
    }
};

class StationSimParticleFit : public ParticleFit<Model, ModelState> {
  public:
    [[nodiscard]] float calculate_particle_fit(const Model &particle, const ModelState &measured_state) const override {
        float distance = 0;

        ModelState particle_state = particle.get_state();
        for (unsigned long i = 0; i < particle_state.agents_location.size(); i++) {
            if (measured_state.agent_active_status.at(i) == AgentStatus::active) {
                distance += powf(particle_state.agents_location.at(i).x - measured_state.agents_location.at(i).x, 2);
                distance += powf(particle_state.agents_location.at(i).y - measured_state.agents_location.at(i).y, 2);
            }
        }

        return std::sqrt(distance);
    }
};

int main() {
    Chronos::Timer timer("timer1");
    timer.start();

    std::shared_ptr<SyntheticDataFeed> synthetic_data_feed = std::make_shared<SyntheticDataFeed>(SyntheticDataFeed());

    std::shared_ptr<InitialiseModelParticles> initialise_model_particles =
        std::make_shared<InitialiseModelParticles>(InitialiseModelParticles());

    std::shared_ptr<StationSimParticleFit> station_sim_particle_fit =
        std::make_shared<StationSimParticleFit>(StationSimParticleFit());

    std::shared_ptr<ParticleFilterStatistics<Model, ModelState>> particle_filter_statistics =
        std::make_shared<ModelStatistics>(ModelStatistics());

    // Setup and run particle filter
    ParticleFilter<Model, ModelState> particle_filter(synthetic_data_feed, initialise_model_particles,
                                                      station_sim_particle_fit, particle_filter_statistics, 100, 100);
    particle_filter.step();

    timer.stop_timer(true);

    auto weighted_mean_errors = particle_filter_statistics->get_weighted_mean_errors();

    std::vector<float> y_int;
    for (const auto &mean : weighted_mean_errors) {
        y_int.emplace_back(mean);
    }

    cxxplot::Plot<float> plot(y_int);
    plot.set_xlabel("Output index");
    plot.set_ylabel("Weighted mean error");
    plot.show();

    return 0;
}