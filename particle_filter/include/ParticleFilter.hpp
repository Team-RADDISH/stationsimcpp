//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef PARTICLE_FILTER_PARTICLEFILTER_HPP
#define PARTICLE_FILTER_PARTICLEFILTER_HPP

#include "Chronos.hpp"
#include "ParticleFilterDataFeed.hpp"
#include "ParticleFilterFileOutput.hpp"
#include "ParticleFilterStatistics.hpp"
#include "ParticleFit.hpp"
#include "ParticlesInitialiser.hpp"
#include "mpi.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

namespace particle_filter {
    template <class ParticleType, class StateType>
    class ParticleFilter {
      private:
        int number_of_particles;
        int resample_window;
        bool multi_step;
        float particle_std;
        bool do_save;
        bool do_resample;

        int steps_run;
        int total_number_of_particle_steps_to_run;
        int window_counter;

        std::shared_ptr<std::mt19937> generator;
        std::normal_distribution<float> float_normal_distribution;

        std::shared_ptr<std::vector<ParticleType>> particles;
        std::vector<float> particles_weights;
        std::shared_ptr<ParticleFilterDataFeed<StateType>> particle_filter_data_feed;
        std::shared_ptr<ParticleFilterStatistics<ParticleType, StateType>> particle_filter_statistics;

        // ParticleFilterFileOutput<StateType> particle_filter_file_output;

        std::shared_ptr<ParticleFit<ParticleType, StateType>> particle_fit;

      public:
        ParticleFilter() = delete;

        explicit ParticleFilter(
            std::shared_ptr<ParticleFilterDataFeed<StateType>> particle_filter_data_feed,
            std::shared_ptr<ParticlesInitialiser<ParticleType>> particles_initialiser,
            std::shared_ptr<ParticleFit<ParticleType, StateType>> particle_fit,
            std::shared_ptr<ParticleFilterStatistics<ParticleType, StateType>> particle_filter_statistics,
            int number_of_particles, int resample_window, int total_number_of_particle_steps_to_run) {
            this->particle_filter_data_feed = particle_filter_data_feed;

            int world_size;
            MPI_Comm_size(MPI_COMM_WORLD, &world_size);

            if (world_size > number_of_particles) {
                std::cout << "Number of particles are less that the MPI processes!" << std::endl;
                MPI_Abort(MPI_COMM_WORLD, 1);
            }

            if (number_of_particles % world_size != 0) {
                std::cout << "Remainder of particle numbers is not zero by MPI processes!" << std::endl;
                MPI_Abort(MPI_COMM_WORLD, 1);
            }

            this->number_of_particles = number_of_particles / world_size;

            this->resample_window = resample_window;
            this->multi_step = true;
            this->particle_std = 0.25;
            this->do_save = true;
            this->do_resample = true;

            steps_run = 0;
            this->total_number_of_particle_steps_to_run = total_number_of_particle_steps_to_run;
            window_counter = 0;

            std::random_device rd;
            std::array<int, std::mt19937::state_size> seed_data;
            std::generate_n(seed_data.data(), seed_data.size(), std::ref(rd));
            std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
            generator = std::make_shared<std::mt19937>(std::mt19937(seq));

            float_normal_distribution = std::normal_distribution<float>(0.0, particle_std);

            particles_weights = std::vector<float>(this->number_of_particles);
            std::fill(particles_weights.begin(), particles_weights.end(), 1.0);

            Chronos::Timer particles_initialisation_timer("Particles initialisation timer");
            particles_initialisation_timer.start();
            particles = particles_initialiser->initialise_particles(this->number_of_particles);
            particles_initialisation_timer.stop_timer(false);
            std::cout << "Finished particle initialisation " << std::endl;
            particles_initialisation_timer.print_elapsed_time();

            // particle_filter_file_output = ParticleFilterFileOutput<StateType>();

            this->particle_fit = particle_fit;
            this->particle_filter_statistics = particle_filter_statistics;
        }

        ~ParticleFilter() = default;

        /// \brief Step Particle Filter
        ///
        /// Loop through process. Predict the base model and particles
        /// forward. If the resample window has been reached, reweight particles
        /// based on distance to base model and resample particles choosing
        /// particles with higher particles_weights. Then save and animate the data. When
        /// done, plot save figures. Note: if the multi_step is True then
        /// predict() is called once, but steps the model forward until the next
        /// window. This is quicker but means that animations and saves will
        /// only report once per window, rather than every iteration
        void step() {
            Chronos::Timer window_timer("window timer");
            window_timer.start();
            while (steps_run < total_number_of_particle_steps_to_run) {
                std::cout << "Step number: " << steps_run << std::endl;
                int number_of_steps = 1;
                if (multi_step) {
                    steps_run += resample_window;
                    number_of_steps = resample_window;
                } else {
                    steps_run++;
                }

                particle_filter_data_feed->get_state();
                predict(number_of_steps);

                if (steps_run % resample_window == 0) {
                    window_counter++;

                    if (particle_filter_statistics != nullptr) {
                        particle_filter_statistics->calculate_statistics(particle_filter_data_feed, *particles,
                                                                         particles_weights);
                    }

                    if (do_resample) {
                        calculate_and_update_particles_weights();
                        update_sample_with_particles();
                        perturb_particles(number_of_steps);
                    }
                }

                window_timer.stop_timer(false);
                std::cout << "Finished windows " << window_counter << std::endl;
                window_timer.print_elapsed_time();
                window_timer.reset();
                window_timer.start();
            }
        }

        /// \brief Step the base model
        ///
        /// Increment time. Step the base model. Use a multiprocessing method to
        /// step particle particles, set the particle states as the agent
        /// locations with some added noise, and reassign the
        /// locations of the particle agents using the new particle
        /// states. We extract the particles and states from the stepped
        /// particles variable.
        /// \param number_of_steps The number of iterations to step (usually either 1, or the  resample window)
        void predict(int number_of_steps) {
            for (int i = 0; i < number_of_steps; i++) {
                particle_filter_data_feed->progress_feed();
            }

#pragma omp parallel for shared(particles)
            for (int i = 0; i < (*particles).size(); i++) {
                step_particle((*particles).at(i), number_of_steps);
            }
        }

        void perturb_particles(int number_of_steps) {
#pragma omp parallel for shared(particles)
            for (int i = 0; i < (*particles).size(); i++) {
                perturb_particle((*particles).at(i), number_of_steps);
            }
        }

        /// \brief Step a particle
        ///
        /// Step a particle, assign the locations of the
        /// agents to the particle state with some noise, and
        /// then use the new particle state to set the location
        /// of the agents.
        ///
        /// \param model Μodel object associated with the particle that needs to be stepped
        /// \param num_iter The number of iterations to step
        void step_particle(ParticleType &particle, int num_iter) {
            for (int i = 0; i < num_iter; i++) {
                particle.step();
            }
        }

        void perturb_particle(ParticleType &particle, int num_iter) {
            for (int i = 0; i < num_iter; i++) {
                particle.perturb_state(particle_std);
            }
        }

        void calculate_and_update_particles_weights() {
            StateType measured_state = particle_filter_data_feed->get_state();

            std::vector<float> distance;
            for (int i = 0; i < (*particles).size(); i++) {
                distance.push_back(particle_fit->calculate_particle_fit((*particles).at(i), measured_state));
            }
        }

        void update_sample_with_particles() {
            int world_rank;
            MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
            int world_size;
            MPI_Comm_size(MPI_COMM_WORLD, &world_size);

            std::vector<int> indexes(number_of_particles * world_size);

            if (world_rank == 0) {
                // Get weights from all processes
                std::vector<std::vector<float>> global_particle_weights(world_size,
                                                                        std::vector<float>(number_of_particles));

                for (int i = 1; i < world_size; i++) {
                    std::vector<float> particles_weights_temp(number_of_particles);

                    MPI_Recv(particles_weights_temp.data(), number_of_particles, MPI_FLOAT, i, 0, MPI_COMM_WORLD,
                             MPI_STATUS_IGNORE);

                    for (int j = 0; j < particles_weights_temp.size(); j++) {
                        global_particle_weights.at(i).at(j) = particles_weights_temp.at(j);
                    }
                }

                double sum = std::reduce(particles_weights.begin(), particles_weights.end(), 0.0);

                for (int i = 1; i < world_size; i++) {
                    for (size_t j = 0; j < global_particle_weights.at(i).size(); j++) {
                        sum = std::reduce(global_particle_weights.at(i).begin(), global_particle_weights.at(i).end(),
                                          sum);
                    }
                }

                // Normalise particle_weights
                std::for_each(particles_weights.begin(), particles_weights.end(),
                              [sum](float &weight) { weight = static_cast<float>(static_cast<double>(weight) / sum); });

                for (int i = 1; i < world_size; i++) {
                    for (size_t j = 0; j < global_particle_weights.at(i).size(); j++) {
                        std::for_each(
                            global_particle_weights.at(i).begin(), global_particle_weights.at(i).end(),
                            [sum](float &weight) { weight = static_cast<float>(static_cast<double>(weight) / sum); });
                    }
                }

                // Calculate cummulative sum
                std::vector<float> cumsum(particles_weights.size() * world_size);
                cumsum.at(0) = particles_weights.at(0);

                for (size_t i = 1; i < particles_weights.size(); i++) {
                    cumsum.at(i) = cumsum.at(i - 1) + particles_weights.at(i);
                }

                for (int i = 1; i < world_size; i++) {
                    for (size_t j = 0; j < particles_weights.size(); j++) {
                        cumsum.at(i * number_of_particles + j) =
                            cumsum.at(i * number_of_particles + j - 1) + global_particle_weights.at(i).at(j);
                    }
                }

                float max_cumsum = *(std::max_element(cumsum.begin(), cumsum.end()));

                // Sampling
                std::uniform_real_distribution<float> dis(0.0, max_cumsum);

                for (int j = 0; j < number_of_particles * world_size; j++) {
                    float u1 = dis(*generator);
                    for (int i = 0; i < number_of_particles * world_size; i++) {
                        if (u1 <= cumsum.at(i)) {
                            indexes.at(j) = i;
                            break;
                        }
                    }
                }

                // send swap indexes vector to all process
                for (int i = 1; i < world_size; i++) {
                    MPI_Send(indexes.data(), number_of_particles * world_size, MPI_INT, i, 0, MPI_COMM_WORLD);
                }
            } else {
                MPI_Send(particles_weights.data(), number_of_particles, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
                MPI_Recv(indexes.data(), number_of_particles * world_size, MPI_INT, 0, 0, MPI_COMM_WORLD,
                         MPI_STATUS_IGNORE);
            }

            std::vector<StateType> particles_states(number_of_particles);
#pragma omp parallel for shared(particles_states, particles)
            for (unsigned long i = 0; i < particles_states.size(); i++) {
                particles_states.at(i) = (*particles).at(i).get_state();
            }

            std::vector<StateType> particle_states_temp(number_of_particles);

            for (unsigned long i = 0; i < indexes.size(); i++) {

                int rank_source = std::floor(i / number_of_particles);
                int rank_destination = std::floor(indexes.at(i) / number_of_particles);

                // Swap is in the current process
                if (rank_source == world_rank && rank_destination == world_rank) {
                    particle_states_temp.at(i - (world_rank * number_of_particles)) =
                        particles_states.at(indexes.at(i));
                }

                // Target is in other process
                if (rank_source == world_rank && rank_destination != world_rank) {
                    // Send the particle state to another process
                    particles_states.at(i - (world_rank * number_of_particles)).mpi_send_state(rank_destination);
                }

                // Source is in other process and target is in the current process
                if (rank_source != world_rank && rank_destination == world_rank) {
                    particle_states_temp.at(indexes.at(i) - (world_rank * number_of_particles))
                        .mpi_receive_state(rank_source);
                }
            }

#pragma omp parallel for shared(particles_states, particles)
            for (int i = 0; i < particle_states_temp.size(); i++) {
                update_agents_locations_of_model(particle_states_temp.at(i), (*particles).at(i));
            }
        }

        void update_agents_locations_of_model(StateType particle_state, ParticleType &particle) {
            particle.set_state(particle_state);
        }

        [[nodiscard]] float get_best_particle() {

            StateType measured_state = particle_filter_data_feed->get_state();

            std::vector<float> distance;
            for (int i = 0; i < (*particles).size(); i++) {
                distance.push_back(particle_fit->calculate_particle_fit((*particles).at(i), measured_state));
            }

            std::vector<float>::iterator result = std::max_element(distance.begin(), distance.end());
            return *result;
        }
    };
} // namespace particle_filter

#endif // PARTICLE_FILTER_PARTICLEFILTER_HPP
