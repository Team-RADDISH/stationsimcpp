//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include "Model.hpp"
#include "ModelParameters.hpp"
#include "ModelPlotting.hpp"
#include "ModelState.hpp"
#include "ModelStatistics.hpp"
#include "ParticleFilter.hpp"
#include "ParticleFilterDataFeed.hpp"
#include "ParticleFit.hpp"
#include "ParticlesInitialiser.hpp"
#include "Timer.hpp"
#include "cxxplot.hpp"
#include <memory>
#include <mpi.h>

using namespace station_sim;

class SyntheticDataFeed : public ParticleFilterDataFeed<ModelState> {
  private:
    std::shared_ptr<std::mt19937> generator;
    std::normal_distribution<float> float_normal_distribution;

  public:
    station_sim::Model base_model;

    SyntheticDataFeed(station_sim::Model base_model) {
        std::random_device rd;
        std::array<int, std::mt19937::state_size> seed_data;
        std::generate_n(seed_data.data(), seed_data.size(), std::ref(rd));
        std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
        generator = std::make_shared<std::mt19937>(std::mt19937(seq));

        float target_model_std = 1.0;
        float_normal_distribution = std::normal_distribution<float>(0.0, powf(target_model_std, 2));

        this->base_model = base_model;
    }
    ~SyntheticDataFeed() override = default;

    void set_to_rank_1_state() {
        ModelState base_model_state = this->get_state();
        this->base_model.set_state(base_model_state);
    }

    void progress_feed() override { base_model.step(); }

    [[nodiscard]] ModelState get_state() override {
        int world_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
        int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);

        ModelState local_state = local_get_state();
        if (world_rank == 0) {
            for (int j = 1; j < world_size; j++) {
                local_state.mpi_send_state(j);
            }
        } else {
            local_state.mpi_receive_state(0);
        }

        return local_state;
    }

    [[nodiscard]] ModelState local_get_state() {
        ModelState model_state;

        std::vector<Point2D> measured_state(base_model.agents.size());
        std::vector<AgentStatus> agent_active_status(base_model.agents.size());

        model_state.agents_desired_location.resize(base_model.agents.size());

        // Add noise to the synthetic target data
        for (unsigned long i = 0; i < base_model.agents.size(); i++) {
            Point2D agent_location = base_model.agents.at(i).get_agent_location();
            measured_state.at(i).x = agent_location.x; // + float_normal_distribution(*generator);
            measured_state.at(i).y = agent_location.y; // + float_normal_distribution(*generator);
            agent_active_status.at(i) = base_model.agents.at(i).getStatus();
            model_state.agents_desired_location.at(i) = base_model.agents.at(i).get_desired_location();
        }
        model_state.agents_location = measured_state;
        model_state.agent_active_status = agent_active_status;

        return model_state;
    }

    void print_statistics() override {
        std::vector<bool> base_model_agents_status = get_base_model_agents_status(base_model);
        long base_model_active_agents = calculate_base_model_active_agents_number(base_model_agents_status);
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
    ModelState base_model_state;

    InitialiseModelParticles(station_sim::Model base_model) { base_model_state = base_model.get_state(); }

    [[nodiscard]] std::shared_ptr<std::vector<Model>> initialise_particles(int number_of_particles) const override {

        ModelParameters model_parameters = ModelParameters();
        model_parameters.set_population_total(40);
        model_parameters.set_do_print(false);

        std::shared_ptr<std::vector<Model>> particles =
            std::make_shared<std::vector<Model>>(std::vector<Model>(number_of_particles));
#pragma omp parallel for shared(particles)
        for (unsigned long i = 0; i < number_of_particles; i++) {
            station_sim::Model model = station_sim::Model(0, model_parameters);
            model.set_state(base_model_state);

            (*particles).at(i) = Model(model);
        }
        return particles;
    }
};

class StationSimParticleFit : public ParticleFit<Model, ModelState> {
  public:
    [[nodiscard]] float calculate_particle_fit(const Model &particle, const ModelState &measured_state) const override {

        int world_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
        int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);

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

void print_weighted_mean_errors(
    std::shared_ptr<ParticleFilterStatistics<Model, ModelState>> particle_filter_statistics) {

    auto weighted_mean_errors = particle_filter_statistics->get_weighted_mean_errors();

    std::vector<float> y_float;
    for (const auto &mean : weighted_mean_errors) {
        y_float.emplace_back(mean);
    }

    cxxplot::Plot<float> plot(y_float);
    plot.set_xlabel("Output index");
    plot.set_ylabel("Weighted mean error");
    plot.show();
}

void print_absolute_mean_errors(
    std::shared_ptr<ParticleFilterStatistics<Model, ModelState>> particle_filter_statistics) {

    auto absolute_mean_errors = particle_filter_statistics->get_absolute_mean_errors();

    std::vector<float> y_float;
    for (const auto &mean : absolute_mean_errors) {
        y_float.emplace_back(mean);
    }

    cxxplot::Plot<float> plot(y_float);
    plot.set_xlabel("Output index");
    plot.set_ylabel("Absolute mean error");
    plot.show();
}

void print_synthetic_data_active_agents(
    std::shared_ptr<ParticleFilterStatistics<Model, ModelState>> particle_filter_statistics) {

    auto synthetic_data_active_agents = particle_filter_statistics->get_active_agents();

    std::vector<float> y_int;
    for (const auto &item : synthetic_data_active_agents) {
        y_int.emplace_back(item);
    }

    cxxplot::Plot<float> plot(y_int);
    plot.set_xlabel("Output index");
    plot.set_ylabel("Synthetic data active agents");
    plot.show();
}

int main() {
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    Chronos::Timer timer("timer1");
    timer.start();

    station_sim::ModelParameters model_parameters = ModelParameters();
    model_parameters.set_population_total(40);
    model_parameters.set_do_print(false);
    station_sim::Model base_model = station_sim::Model(0, model_parameters);

    std::shared_ptr<SyntheticDataFeed> synthetic_data_feed =
        std::make_shared<SyntheticDataFeed>(SyntheticDataFeed(base_model));
    synthetic_data_feed->set_to_rank_1_state();

    std::shared_ptr<InitialiseModelParticles> initialise_model_particles =
        std::make_shared<InitialiseModelParticles>(InitialiseModelParticles(synthetic_data_feed->base_model));

    std::shared_ptr<StationSimParticleFit> station_sim_particle_fit =
        std::make_shared<StationSimParticleFit>(StationSimParticleFit());

    std::shared_ptr<ParticleFilterStatistics<Model, ModelState>> particle_filter_statistics =
        std::make_shared<ModelStatistics>(ModelStatistics());

    // Setup and run particle filter
    ParticleFilter<Model, ModelState> particle_filter(synthetic_data_feed, initialise_model_particles,
                                                      station_sim_particle_fit, particle_filter_statistics, 1000, 1);
    particle_filter.step();

    timer.stop_timer(true);

    if (world_rank == 0) {
        print_synthetic_data_active_agents(particle_filter_statistics);
        print_weighted_mean_errors(particle_filter_statistics);
        print_absolute_mean_errors(particle_filter_statistics);
    }

    MPI_Finalize();
    return 0;
}
