//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_PARTICLEFILTER_HPP
#define STATIONSIM_PARTICLEFILTER_HPP

#include "HelpFunctions.hpp"
#include "ParticleFilterDataFeed.hpp"
#include "ParticleFilterParameters.hpp"
#include "ParticleFilterStatistics.hpp"
#include "ParticlesInitialiser.hpp"
#include "model/Model.hpp"
#include "model/MultipleModelsRun.hpp"
#include "stationsim_export.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

namespace station_sim {
    template <class ParticleType, class StateType>
    class STATIONSIM_EXPORT ParticleFilter {
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

        std::vector<ParticleType> particles;
        std::vector<StateType> particles_states;
        std::vector<float> particles_weights;
        std::shared_ptr<ParticleFilterDataFeed<StateType>> particle_filter_data_feed;
        ParticleFilterStatistics<ParticleType, StateType> particle_filter_statistics;

      public:
        ParticleFilter() = delete;

        explicit ParticleFilter(std::shared_ptr<ParticleFilterDataFeed<StateType>> particle_filter_data_feed,
                                std::shared_ptr<ParticlesInitialiser<ParticleType>> particles_initialiser) {

            this->particle_filter_data_feed = particle_filter_data_feed;

            this->number_of_particles = 2000;
            this->resample_window = 100;
            this->multi_step = true;
            this->particle_std = 0.5;
            this->do_save = true;
            this->do_resample = true;

            steps_run = 0;
            total_number_of_particle_steps_to_run = 1000;
            window_counter = 0;

            std::random_device r;
            generator = std::make_shared<std::mt19937>(std::mt19937(r()));

            float_normal_distribution = std::normal_distribution<float>(0.0, particle_std * number_of_particles);

            particles_states.resize(number_of_particles);

            particles_weights = std::vector<float>(number_of_particles);
            std::fill(particles_weights.begin(), particles_weights.end(), 1.0);

            particles = particles_initialiser->initialise_particles(number_of_particles);
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

                if (std::any_of(particles.cbegin(), particles.cend(),
                                [](const ParticleType &particle) { return particle.is_active(); })) {

                    predict(number_of_steps);

                    if (steps_run % resample_window == 0) {
                        window_counter++;

                        particle_filter_statistics.calculate_statistics(*particle_filter_data_feed, particles,
                                                                        particles_weights);

                        if (do_resample) {
                            reweight();
                            resample();
                        }
                    }

                    window_timer.stop_timer(false);
                    std::cout << "Finished windows " << window_counter << std::endl;
                    window_timer.print_elapsed_time();
                    window_timer.reset();
                    window_timer.start();
                }
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
        void predict(int number_of_steps = 1) {
            for (int i = 0; i < number_of_steps; i++) {
                particle_filter_data_feed->run_model();
            }

#pragma omp parallel for shared(particles)
            for (int i = 0; i < particles.size(); i++) {
                step_particle(particles[i], number_of_steps * number_of_particles);
            }

#pragma omp parallel for shared(particles_states, particles)
            for (unsigned long i = 0; i < particles_states.size(); i++) {
                particles_states[i] = particles[i].get_state();
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
            particle.reseed_random_number_generator();
            for (int i = 0; i < num_iter; i++) {
                particle.step();
            }

            std::vector<float> state = particle.get_state();
            std::for_each(state.begin(), state.end(), [&](float &x) { x += float_normal_distribution(*generator); });
            particle.set_state(state);
        }

        void reweight() {
            StateType measured_state = particle_filter_data_feed->get_state();

            std::vector<float> distance;
            for (int i = 0; i < particles.size(); i++) {
                distance.push_back(calculate_particle_fit(particles[i], measured_state));
            }

            std::transform(distance.begin(), distance.end(), particles_weights.begin(), [](float distance) -> float {
                return static_cast<float>(1.0 / (pow((static_cast<double>(distance) + 1e-9), 2)));
            });

            double sum = std::reduce(particles_weights.begin(), particles_weights.end(), 0.0);
            std::for_each(particles_weights.begin(), particles_weights.end(),
                          [sum](float &weight) { weight = static_cast<float>(static_cast<double>(weight) / sum); });
        }

        float calculate_particle_fit(const ParticleType &particle, const StateType &measured_state) {
            float distance = 0;

            StateType particle_state = particle.get_state();
            for (int i = 0; i < particle_state.size(); i++) {
                distance += powf(particle_state[i] - measured_state[i], 2);
            }

            return std::sqrt(distance);
        }

        void resample() {
            std::vector<float> offset_partition =
                HelpFunctions::evenly_spaced_values_within_interval(0, number_of_particles, 1);

            std::uniform_real_distribution<float> dis(0.0, 1.0);
            std::for_each(offset_partition.begin(), offset_partition.end(),
                          [&](float &value) { value = (value + dis(*generator)) / number_of_particles; });

            std::vector<float> cumsum(particles_weights.size());
            cumsum[0] = particles_weights[0];
            for (size_t i = 1; i < particles_weights.size(); i++) {
                cumsum[i] = cumsum[i - 1] + particles_weights[i];
            }

            std::vector<int> indexes(number_of_particles);

            {
                int i = 0;
                int j = 0;
                while (i < number_of_particles) {
                    if (offset_partition[i] < cumsum[j]) {
                        indexes[i] = j;
                        i++;
                    } else {
                        j++;
                    }
                }
            }

            std::vector<float> weights_temp(particles_weights);
            for (int i = 0; i < indexes.size(); i++) {
                weights_temp[i] = particles_weights[i];
            }

            for (int i = 0; i < indexes.size(); i++) {
                particles_weights[i] = weights_temp[indexes[i]];
            }

            std::vector<StateType> particle_states_temp(particles_states);
            for (int i = 0; i < indexes.size(); i++) {
                particle_states_temp[i] = particles_states[i];
            }
            for (int i = 0; i < indexes.size(); i++) {
                particles_states[i] = particle_states_temp[indexes[i]];
            }

            for (int i = 0; i < indexes.size(); i++) {
                update_agents_locations_of_model(particles_states[i], particles[i]);
            }
        }

        void update_agents_locations_of_model(StateType particle_state, ParticleType &particle) {
            particle.set_state(particle_state);
        }

        [[nodiscard]] const ParticleFilterStatistics<ParticleType, StateType> &get_particle_filter_statistics() const {
            return particle_filter_statistics;
        }
    };
} // namespace station_sim

#endif // STATIONSIM_PARTICLEFILTER_HPP
