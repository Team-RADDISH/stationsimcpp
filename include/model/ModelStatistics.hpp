//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_MODELSTATISTICS_HPP
#define STATIONSIM_MODELSTATISTICS_HPP

#include "mpi.h"
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

            this->particle_filter_data_feed = particle_filter_data_feed;

            std::vector<ModelState> particles_states;
            for (const auto &particle : particles) {
                particles_states.push_back(particle.get_state());
            }

            std::vector<float> absolute_mean = calculate_absolute_average(particles_states);
            std::vector<float> weighted_mean = calculate_weighted_average(particles_states, particles_weights);

            this->absolute_means_states.push_back(absolute_mean);
            this->weighted_means_states.push_back(weighted_mean);

            this->absolute_mean_errors.push_back(calculate_absolute_mean_error(absolute_mean));
            this->weighted_mean_errors.push_back(calculate_weighted_mean_error(weighted_mean));

            this->active_agents.push_back(calculate_number_of_active_agents(particle_filter_data_feed));
        }

        // For each active agent state (i.e. x and y) calculate the absolute average from all particles
        [[nodiscard]] std::vector<float>
        calculate_absolute_average(const std::vector<ModelState> &particles_states) override {
            ModelState data_feed_state = particle_filter_data_feed->get_state();

            // calculate the number of active agents in the data feed
            long number_of_active_agents_in_datafeed =
                std::count_if(data_feed_state.agent_active_status.begin(), data_feed_state.agent_active_status.end(),
                              [](auto i) { return i == AgentStatus::active; });

            // size of vectors is the number of active agent * the number of space dimensions (i.e. x and y)
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

            // Receive sum vectors from other processes
            int world_rank;
            MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
            int world_size;
            MPI_Comm_size(MPI_COMM_WORLD, &world_size);

            if (world_rank == 0) {
                std::vector<float> local_sum(sum.size());

                for (int i = 1; i < world_size; i++) {
                    MPI_Recv(local_sum.data(), static_cast<int>(local_sum.size()), MPI_FLOAT, i, 0, MPI_COMM_WORLD,
                             MPI_STATUS_IGNORE);

                    std::transform(sum.begin(), sum.end(), local_sum.begin(), sum.begin(), std::plus<float>());
                }

                std::vector<float> absolute_mean(sum.size());
                for (unsigned long i = 0; i < absolute_mean.size(); i++) {
                    absolute_mean.at(i) = sum.at(i) / static_cast<float>(particles_states.size() * world_size);
                }
                return absolute_mean;
            } else {
                MPI_Send(sum.data(), static_cast<int>(sum.size()), MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
                return std::vector<float>(sum.size(), 0);
            }
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

            for (unsigned long i = 0; i < particles_states.size(); i++) {
                unsigned long active_agent_index = 0;
                for (unsigned long j = 0; j < particles_states.at(i).agents_location.size(); j++) {
                    if (data_feed_state.agent_active_status.at(j) == AgentStatus::active) {
                        sum.at(active_agent_index) += particles_states.at(i).agents_location.at(j).x * weights.at(i);
                        active_agent_index++;
                        sum.at(active_agent_index) += particles_states.at(i).agents_location.at(j).y * weights.at(i);
                        active_agent_index++;
                    }
                }
            }

            ////////////////////////////////////////////////////////////////////////////////////////////////////////////
            // Receive sum vectors from other processes
            int world_rank;
            MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
            int world_size;
            MPI_Comm_size(MPI_COMM_WORLD, &world_size);

            if (world_rank == 0) {
                std::vector<float> local_sum(sum.size());
                for (int i = 1; i < world_size; i++) {
                    MPI_Recv(local_sum.data(), static_cast<int>(local_sum.size()), MPI_FLOAT, i, 0, MPI_COMM_WORLD,
                             MPI_STATUS_IGNORE);
                    std::transform(sum.begin(), sum.end(), local_sum.begin(), sum.begin(), std::plus<float>());
                }

                float sum_weights = static_cast<float>(std::reduce(weights.begin(), weights.end(), 0.0));
                for (int i = 1; i < world_size; i++) {
                    float temp_sum_weights;
                    MPI_Recv(&temp_sum_weights, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    sum_weights += temp_sum_weights;
                }

                for (unsigned long i = 0; i < sum.size(); i++) {
                    weighted_mean.at(i) = sum.at(i) / sum_weights;
                }
                return weighted_mean;
            } else {
                MPI_Send(sum.data(), static_cast<int>(sum.size()), MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
                float sum_weights = static_cast<float>(std::reduce(weights.begin(), weights.end(), 0.0));
                MPI_Send(&sum_weights, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
                return std::vector<float>(sum.size(), 0);
            }
        }

        [[nodiscard]] float calculate_absolute_mean_error(std::vector<float> absolute_mean) override {
            float sum = 0;

            ModelState data_feed_state = particle_filter_data_feed->get_state();

            unsigned long mean_index = 0;
            for (unsigned int i = 0; i < data_feed_state.agent_active_status.size(); i++) {
                if (data_feed_state.agent_active_status.at(i) == AgentStatus::active) {
                    sum += powf(absolute_mean.at(mean_index) - data_feed_state.agents_location.at(i).x, 2);
                    mean_index++;
                    sum += powf(absolute_mean.at(mean_index) - data_feed_state.agents_location.at(i).y, 2);
                    mean_index++;
                }
            }

            return std::sqrt(sum);
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
