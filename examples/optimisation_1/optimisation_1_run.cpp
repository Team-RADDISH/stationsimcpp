//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include "Chronos.hpp"
#include "ParticleFilter.hpp"
#include "ParticleFilterDataFeed.hpp"
#include "ParticleFit.hpp"
#include "ParticlesInitialiser.hpp"
#include "SphereFunction.hpp"
#include "SphereFunctionState.hpp"
#include "SphereFunctionStatistics.hpp"
#include "cxxplot.hpp"
#include "mpi.h"
#include <memory>
#include <random>

using namespace station_sim;

class SyntheticDataFeed : public ParticleFilterDataFeed<SphereFunctionState> {
  public:
    SyntheticDataFeed() {}
    ~SyntheticDataFeed() override = default;

    void progress_feed() override {}

    [[nodiscard]] SphereFunctionState get_state() override {
        SphereFunctionState local_state;
        local_state.x = 0;
        local_state.y = 0;

        return local_state;
    }

    void print_statistics() override {}
};

class InitialiseModelParticles : public ParticlesInitialiser<SphereFunction> {
  public:
    InitialiseModelParticles() = default;

    [[nodiscard]] std::shared_ptr<std::vector<SphereFunction>>
    initialise_particles(int number_of_particles) const override {

        std::shared_ptr<std::vector<SphereFunction>> particles =
            std::make_shared<std::vector<SphereFunction>>(std::vector<SphereFunction>(number_of_particles));

        std::random_device rd;
        std::array<int, std::mt19937::state_size> seed_data;
        std::generate_n(seed_data.data(), seed_data.size(), std::ref(rd));
        std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
        std::shared_ptr<std::mt19937> generator = std::make_shared<std::mt19937>(std::mt19937(seq));

        std::uniform_real_distribution<float> distribution = std::uniform_real_distribution<float>(-50, 50);

#pragma omp parallel for shared(particles)
        for (unsigned long i = 0; i < number_of_particles; i++) {
            (*particles).at(i) = SphereFunction(distribution(*generator), distribution(*generator));
        }
        return particles;
    }
};

class StationSimParticleFit : public ParticleFit<SphereFunction, SphereFunctionState> {
  public:
    [[nodiscard]] float calculate_particle_fit(const SphereFunction &particle,
                                               const SphereFunctionState &measured_state) const override {

        int world_rank;
        MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
        int world_size;
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);

        float distance = 0;

        SphereFunctionState particle_state = particle.get_state();

        distance += powf(particle_state.x, 2);
        distance += powf(particle_state.y, 2);

        return static_cast<float>(1.0 / (distance + 1e-9));
    }
};

void print_distance(std::shared_ptr<SphereFunctionStatistics> particle_filter_statistics) {

    std::vector<float> distances = particle_filter_statistics->best_distance;

    std::vector<float> y_float;
    for (const auto &distance : distances) {
        y_float.emplace_back(distance);
        std::cout << distance << std::endl;
    }

    cxxplot::Plot<float> plot(y_float);
    plot.set_xlabel("Output index");
    plot.set_ylabel("Best distance");
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

    std::shared_ptr<SyntheticDataFeed> synthetic_data_feed = std::make_shared<SyntheticDataFeed>(SyntheticDataFeed());

    std::shared_ptr<InitialiseModelParticles> initialise_model_particles =
        std::make_shared<InitialiseModelParticles>(InitialiseModelParticles());

    std::shared_ptr<StationSimParticleFit> station_sim_particle_fit =
        std::make_shared<StationSimParticleFit>(StationSimParticleFit());

    std::shared_ptr<SphereFunctionStatistics> particle_filter_statistics =
        std::make_shared<SphereFunctionStatistics>(SphereFunctionStatistics());

    // Setup and run particle filter
    ParticleFilter<SphereFunction, SphereFunctionState> particle_filter(synthetic_data_feed, initialise_model_particles,
                                                                        station_sim_particle_fit,
                                                                        particle_filter_statistics, 100, 1, 100000);
    particle_filter.step();

    timer.stop_timer(true);

    if (world_rank == 0) {
        std::cout << particle_filter.get_best_particle() << std::endl;
        print_distance(particle_filter_statistics);
    }

    MPI_Finalize();
    return 0;
}
