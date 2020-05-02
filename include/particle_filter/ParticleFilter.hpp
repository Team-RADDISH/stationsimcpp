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
        int number_of_runs;
        int resample_window;
        bool multi_step;
        float particle_std;
        float target_model_std;
        int agents_to_visualise;
        bool do_save;
        bool do_resample;
        bool plot_save;
        bool do_ani;
        bool show_ani;

        int steps_run;
        int total_number_of_particle_steps_to_run;
        int window_counter;

        std::vector<float> particles_weights;
        std::vector<std::vector<Point2D>> locations;

        std::shared_ptr<std::mt19937> generator;
        std::normal_distribution<float> float_normal_distribution;

        ParticleType base_model;
        std::vector<ParticleType> particles;
        MultipleModelsRun multiple_models_run;

        std::shared_ptr<ParticleFilterDataFeed<StateType>> particle_filter_data_feed;

      public:
        ParticleFilter() = delete;

        explicit ParticleFilter(ParticleType base_model,
                                std::shared_ptr<ParticleFilterDataFeed<StateType>> particle_filter_data_feed) {

            this->particle_filter_data_feed = particle_filter_data_feed;

            this->number_of_particles = 100;
            this->number_of_runs = 20;
            this->resample_window = 10;
            this->multi_step = true;
            this->particle_std = 0.5;
            this->target_model_std = 1.0;
            this->agents_to_visualise = 10;
            this->do_save = true;
            this->plot_save = false;
            this->do_ani = false;
            this->show_ani = false;
            this->do_resample = true;

            steps_run = 0;
            total_number_of_particle_steps_to_run = 100;
            window_counter = 0;

            std::random_device r;
            generator = std::make_shared<std::mt19937>(std::mt19937(r()));
            float_normal_distribution = std::normal_distribution<float>(0.0, particle_std);

            this->base_model = base_model;

            locations.resize(number_of_particles);
            std::for_each(locations.begin(), locations.end(), [&base_model](std::vector<Point2D> &point_2d) {
                point_2d = base_model.get_agents_location();
            });

            particles_weights = std::vector<float>(number_of_particles);
            std::fill(particles_weights.begin(), particles_weights.end(), 1.0);

            initialise_particles();
        }

        ~ParticleFilter() = default;

        void initialise_particles() {
            station_sim::ModelParameters model_parameters;
            model_parameters.set_population_total(base_model.get_model_parameters().get_population_total());
            model_parameters.set_do_print(false);
            for (int i = 0; i < number_of_particles; i++) {
                station_sim::Model model(i, model_parameters);
                // multiple_models_run.add_model_and_model_parameters(model);
                particles.push_back(model);
            }
        }

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
                                [](const ParticleType &particle) { return particle.get_status(); })) {

                    predict(number_of_steps);

                    if (steps_run % resample_window == 0) {
                        window_counter++;

                        calculate_statistics();

                        if (do_resample) {
                            reweight();
                            resample();
                        }
                    }
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
                base_model.step();
                particle_filter_data_feed->run_model();
            }

            for (auto &model : particles) {
                step_particle(model, number_of_steps * number_of_particles, particle_std * number_of_particles);
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
        /// \param particle_std Τhe particle noise standard deviation
        void step_particle(ParticleType &model, int num_iter, float particle_std) {
            model.reseed_random_number_generator();
            for (int i = 0; i < num_iter; i++) {
                model.step();
            }

            std::vector<float> state = model.get_state();

            std::for_each(state.begin(), state.end(), [&](float &x) { x += float_normal_distribution(*generator); });

            model.set_state(state);
        }

        void reweight() {

            // Add noise to the synthetic target data
            float_normal_distribution = std::normal_distribution<float>(0.0, powf(target_model_std, 2));
            std::vector<Point2D> measured_state(base_model.agents.size());
            for (unsigned long i = 0; i < base_model.agents.size(); i++) {
                Point2D agent_location = base_model.agents[i].get_agent_location();
                measured_state.at(i).x = agent_location.x + float_normal_distribution(*generator);
                measured_state.at(i).y = agent_location.y + float_normal_distribution(*generator);
            }

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

        float calculate_particle_fit(const Model &model, const std::vector<Point2D> &measured_state) {
            float distance = 0;

            for (int i = 0; i < model.agents.size(); i++) {
                distance += std::sqrt(powf(model.agents[i].get_agent_location().x - measured_state[i].x, 2) +
                                      powf(model.agents[i].get_agent_location().y - measured_state[i].y, 2));
            }

            return distance;
        }

        void resample() {
            std::vector<float> offset_partition =
                HelpFunctions::evenly_spaced_values_within_interval(0, number_of_particles, 1);

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dis(0.0, 1.0);
            std::for_each(offset_partition.begin(), offset_partition.end(),
                          [&](float &value) { value = (value + dis(gen)) / number_of_particles; });

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

            std::vector<std::vector<Point2D>> locations_temp(locations);
            for (int i = 0; i < indexes.size(); i++) {
                locations_temp[i] = locations[i];
            }
            for (int i = 0; i < indexes.size(); i++) {
                locations[i] = locations_temp[indexes[i]];
            }

            for (int i = 0; i < indexes.size(); i++) {
                update_agents_locations_of_model(locations[i], particles[i]);
            }
        }

        void update_agents_locations_of_model(std::vector<Point2D> locations, Model &model) {
            for (int i = 0; i < model.agents.size(); i++) {
                model.agents[i].set_agent_location(locations[i]);
            }
        }

        void calculate_statistics() {
            // calculate number of active agents in base model
            int base_model_active_agents = 0;
            for (const Agent &agent : base_model.agents) {
                if (agent.getStatus() == AgentStatus::active) {
                    base_model_active_agents++;
                }
            }

            std::cout << "Base model active agents: " << base_model_active_agents << std::endl;
        }
    };
} // namespace station_sim

#endif // STATIONSIM_PARTICLEFILTER_HPP
