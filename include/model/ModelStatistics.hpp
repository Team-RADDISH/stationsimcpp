//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_MODELSTATISTICS_HPP
#define STATIONSIM_MODELSTATISTICS_HPP

#include "particle_filter/ParticleFilterDataFeed.hpp"
#include "particle_filter/ParticleFilterStatistics.hpp"
#include "stationsim_export.h"
#include <algorithm>

namespace station_sim {
    class STATIONSIM_EXPORT ModelStatistics : public ParticleFilterStatistics<Model, ModelState> {
      public:
        ModelStatistics() = default;
        ~ModelStatistics() override = default;

        void calculate_statistics(const std::shared_ptr<ParticleFilterDataFeed<ModelState>> particle_filter_data_feed,
                                  const std::vector<Model> &particles,
                                  const std::vector<float> &particles_weights) override {

            std::cout << "Particle filter general statistics:" << std::endl;

            this->particle_filter_data_feed = particle_filter_data_feed;

            std::vector<ModelState> particles_states;
            for (const auto &particle : particles) {
                particles_states.push_back(particle.get_state());
            }

            std::vector<float> unweighted_mean = calculate_unweighted_average(particles_states);
            std::vector<float> weighted_mean = calculate_weighted_average(particles_states, particles_weights);

            this->weighted_means_states.push_back(weighted_mean);

            this->weighted_mean_errors.push_back(calculate_weighted_mean_error(weighted_mean));

            this->active_agents.push_back(calculate_number_of_active_agents(particle_filter_data_feed));
        }

        // For each active agent state (i.e. x and y) calculate the unweighted average from all particles
        [[nodiscard]] std::vector<float>
        calculate_unweighted_average(const std::vector<ModelState> &particles_states) override {
            ModelState data_feed_state = particle_filter_data_feed->get_state();

            // calculate the number of active agents in the data feed
            long number_of_active_agents_in_datafeed =
                std::count_if(data_feed_state.agent_active_status.begin(), data_feed_state.agent_active_status.end(),
                              [](auto i) { return i == AgentStatus::active; });

            // size of vectors is the number of active agent * the number of space dimensions (i.e. x and y)
            std::vector<float> unweighted_mean(static_cast<unsigned long>(number_of_active_agents_in_datafeed * 2));
            std::vector<float> sum(static_cast<unsigned long>(number_of_active_agents_in_datafeed * 2));
            std::fill(sum.begin(), sum.end(), 0);

            for (const ModelState &particle_state : particles_states) {
                unsigned long active_agent_index = 0;
                for (unsigned long j = 0; j < particle_state.agents_location.size(); j++) {
                    if (data_feed_state.agent_active_status[j] == AgentStatus::active) {
                        sum[active_agent_index] += particle_state.agents_location[j].x;
                        active_agent_index++;
                        sum[active_agent_index] += particle_state.agents_location[j].y;
                        active_agent_index++;
                    }
                }
            }

            for (unsigned long i = 0; i < unweighted_mean.size(); i++) {
                unweighted_mean[i] = sum[i] / static_cast<float>(sum.size());
            }

            return unweighted_mean;
        }

        // For each active agent state (i.e. x and y) calculate the weighted average from all particles
        [[nodiscard]] std::vector<float> calculate_weighted_average(const std::vector<ModelState> &particles_states,
                                                                    const std::vector<float> &weights) override {
            ModelState data_feed_state = particle_filter_data_feed->get_state();

            // calculate the number of active agents in the data feed
            long number_of_active_agents_in_datafeed =
                std::count_if(data_feed_state.agent_active_status.begin(), data_feed_state.agent_active_status.end(),
                              [](auto i) { return i == AgentStatus::active; });

            // size of vectors is the number of active agent * the number of space dimensions (i.e. x and y)
            std::vector<float> weighted_mean(static_cast<unsigned long>(number_of_active_agents_in_datafeed * 2));
            std::vector<float> sum(static_cast<unsigned long>(number_of_active_agents_in_datafeed * 2));
            std::fill(sum.begin(), sum.end(), 0);

            for (const ModelState &particle_state : particles_states) {
                unsigned long active_agent_index = 0;
                for (unsigned long j = 0; j < particle_state.agents_location.size(); j++) {
                    if (data_feed_state.agent_active_status.at(j) == AgentStatus::active) {
                        sum.at(active_agent_index) += particle_state.agents_location.at(j).x;
                        active_agent_index++;
                        sum.at(active_agent_index) += particle_state.agents_location.at(j).y;
                        active_agent_index++;
                    }
                }
            }

            float sum_weights = static_cast<float>(std::reduce(weights.begin(), weights.end(), 0.0));

            for (unsigned long i = 0; i < weighted_mean.size(); i++) {
                weighted_mean.at(i) = sum.at(i) * weights.at(i) / sum_weights;
            }

            return weighted_mean;
        }

        [[nodiscard]] float calculate_weighted_mean_error(std::vector<float> weighted_mean) override {
            float sum = 0;

            ModelState data_feed_state = particle_filter_data_feed->get_state();
            unsigned long weighted_mean_index = 0;
            for (unsigned int i = 0; i < data_feed_state.agent_active_status.size(); i++) {
                if (data_feed_state.agent_active_status.at(i) == AgentStatus::active) {
                    sum += powf(weighted_mean.at(weighted_mean_index) - data_feed_state.agents_location.at(i).x, 2);
                    weighted_mean_index++;
                    sum += powf(weighted_mean.at(weighted_mean_index) - data_feed_state.agents_location.at(i).y, 2);
                    weighted_mean_index++;
                }
            }

            return std::sqrt(sum);
        }

        //        std::vector<float> calculate_variance(const Model &base_model, const std::vector<Model> &models,
        //                                              const std::vector<float> &weights,
        //                                              const std::vector<bool> &base_model_agents_status,
        //                                              const long base_model_active_agents,
        //                                              const std::vector<float> &unweighted_mean) override {
        //
        //            std::vector<float> variance(static_cast<unsigned long>(models.size()));
        //
        //            std::vector<std::array<float, 2>> sum(static_cast<unsigned long>(models.size()));
        //            std::fill(sum.begin(), sum.end(), std::array<float, 2>{0, 0});
        //
        //            for (unsigned long i = 0; i < models.size(); i++) {
        //                for (unsigned long j = 0; j < models[i].agents.size(); j++) {
        //                    if (base_model_agents_status[j]) {
        //                        sum[i][0] = sum[i][0] + (models[i].agents[j].get_agent_location().x);
        //                        sum[i][1] = sum[i][1] + models[i].agents[j].get_agent_location().y;
        //                    }
        //                }
        //            }
        //
        //            return variance;
        //        }

        [[nodiscard]] long calculate_number_of_active_agents(
            const std::shared_ptr<ParticleFilterDataFeed<ModelState>> particle_filter_data_feed) const {

            ModelState data_feed_state = particle_filter_data_feed->get_state();
            return std::count_if(data_feed_state.agent_active_status.begin(), data_feed_state.agent_active_status.end(),
                                 [](auto i) { return i == AgentStatus::active; });
        }
    };
} // namespace station_sim

#endif // STATIONSIM_MODELSTATISTICS_HPP
