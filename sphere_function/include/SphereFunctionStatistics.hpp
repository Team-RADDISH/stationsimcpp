//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_SPHEREFUNCTIONSTATISTICS_HPP
#define STATIONSIM_SPHEREFUNCTIONSTATISTICS_HPP

#include "ParticleFilterDataFeed.hpp"
#include "ParticleFilterStatistics.hpp"
#include "SphereFunction.hpp"
#include "mpi.h"
#include <algorithm>

namespace station_sim {
    class SphereFunctionStatistics : public ParticleFilterStatistics<SphereFunction, SphereFunctionState> {
      public:
        SphereFunctionStatistics() = default;
        ~SphereFunctionStatistics() override = default;

        std::vector<float> best_distance;

        void calculate_statistics(
            const std::shared_ptr<ParticleFilterDataFeed<SphereFunctionState>> particle_filter_data_feed,
            const std::vector<SphereFunction> &particles, const std::vector<float> &particles_weights) override {

            this->particle_filter_data_feed = particle_filter_data_feed;

            std::vector<SphereFunctionState> particles_states;
            for (const auto &particle : particles) {
                particles_states.push_back(particle.get_state());
            }

            std::vector<float> distance;
            for (int i = 0; i < (particles_states).size(); i++) {
                distance.push_back(powf(particles_states.at(i).x, 2) + powf(particles_states.at(i).y, 2));
            }

            auto iter = std::max_element(distance.begin(), distance.end());
            best_distance.push_back(*iter);
        }

        // For each active agent state (i.e. x and y) calculate the absolute average from all particles
        [[nodiscard]] std::vector<float>
        calculate_absolute_average(const std::vector<SphereFunctionState> &particles_states) override {
            SphereFunctionState data_feed_state = particle_filter_data_feed->get_state();

            //                    // calculate the number of active agents in the data feed
            //                    long number_of_active_agents_in_datafeed =
            //                        std::count_if(data_feed_state.agent_active_status.begin(),
            //                        data_feed_state.agent_active_status.end(),
            //                                      [](auto i) { return i == AgentStatus::active; });
            //
            //                    // size of vectors is the number of active agent * the number of space
            //                    dimensions (i.e. x and y) std::vector<float> sum(static_cast<unsigned
            //                    long>(number_of_active_agents_in_datafeed * 2)); std::fill(sum.begin(),
            //                    sum.end(), 0);
            //
            //                    for (const SphereFunctionState &particle_state : particles_states) {
            //                        unsigned long active_agent_index = 0;
            //                        for (unsigned long j = 0; j < particle_state.agents_location.size(); j++)
            //                        {
            //                            if (data_feed_state.agent_active_status.at(j) == AgentStatus::active)
            //                            {
            //                                sum.at(active_agent_index) +=
            //                                particle_state.agents_location.at(j).x; active_agent_index++;
            //                                sum.at(active_agent_index) +=
            //                                particle_state.agents_location.at(j).y; active_agent_index++;
            //                            }
            //                        }
            //                    }
            //
            //                    // Receive sum vectors from other processes
            //                    int world_rank;
            //                    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
            //                    int world_size;
            //                    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
            //
            //                    if (world_rank == 0) {
            //                        std::vector<float> local_sum(sum.size());
            //
            //                        for (int i = 1; i < world_size; i++) {
            //                            MPI_Recv(local_sum.data(), static_cast<int>(local_sum.size()),
            //                            MPI_FLOAT, i, 0, MPI_COMM_WORLD,
            //                                     MPI_STATUS_IGNORE);
            //
            //                            std::transform(sum.begin(), sum.end(), local_sum.begin(), sum.begin(),
            //                            std::plus<float>());
            //                        }
            //
            //                        std::vector<float> absolute_mean(sum.size());
            //                        for (unsigned long i = 0; i < absolute_mean.size(); i++) {
            //                            absolute_mean.at(i) = sum.at(i) /
            //                            static_cast<float>(particles_states.size() * world_size);
            //                        }
            //                        return absolute_mean;
            //                    } else {
            //                        MPI_Send(sum.data(), static_cast<int>(sum.size()), MPI_FLOAT, 0, 0,
            //                        MPI_COMM_WORLD); return std::vector<float>(sum.size(), 0);
            //                    }
        }

        // For each active agent state (i.e. x and y) calculate the weighted average from all particles
        [[nodiscard]] std::vector<float>
        calculate_weighted_average(const std::vector<SphereFunctionState> &particles_states,
                                   const std::vector<float> &weights) override {
            SphereFunctionState data_feed_state = particle_filter_data_feed->get_state();

            //                    // calculate the number of active agents in the data feed
            //                    long number_of_active_agents_in_datafeed =
            //                        std::count_if(data_feed_state.agent_active_status.begin(),
            //                        data_feed_state.agent_active_status.end(),
            //                                      [](auto i) { return i == AgentStatus::active; });
            //
            //                    // size of vectors is the number of active agent * the number of space dimensions
            //                    (i.e. x and y) std::vector<float> weighted_mean(static_cast<unsigned
            //                    long>(number_of_active_agents_in_datafeed * 2)); std::vector<float>
            //                    sum(static_cast<unsigned long>(number_of_active_agents_in_datafeed * 2));
            //                    std::fill(sum.begin(), sum.end(), 0);
            //
            //                    for (unsigned long i = 0; i < particles_states.size(); i++) {
            //                        unsigned long active_agent_index = 0;
            //                        for (unsigned long j = 0; j < particles_states.at(i).agents_location.size(); j++)
            //                        {
            //                            if (data_feed_state.agent_active_status.at(j) == AgentStatus::active) {
            //                                sum.at(active_agent_index) +=
            //                                particles_states.at(i).agents_location.at(j).x * weights.at(i);
            //                                active_agent_index++; sum.at(active_agent_index) +=
            //                                particles_states.at(i).agents_location.at(j).y * weights.at(i);
            //                                active_agent_index++;
            //                            }
            //                        }
            //                    }
            //
            //                    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
            //                    // Receive sum vectors from other processes
            //                    int world_rank;
            //                    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
            //                    int world_size;
            //                    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
            //
            //                    if (world_rank == 0) {
            //                        std::vector<float> local_sum(sum.size());
            //                        for (int i = 1; i < world_size; i++) {
            //                            MPI_Recv(local_sum.data(), static_cast<int>(local_sum.size()), MPI_FLOAT, i,
            //                            0, MPI_COMM_WORLD,
            //                                     MPI_STATUS_IGNORE);
            //                            std::transform(sum.begin(), sum.end(), local_sum.begin(), sum.begin(),
            //                            std::plus<float>());
            //                        }
            //
            //                        float sum_weights = static_cast<float>(std::reduce(weights.begin(), weights.end(),
            //                        0.0)); for (int i = 1; i < world_size; i++) {
            //                            float temp_sum_weights;
            //                            MPI_Recv(&temp_sum_weights, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD,
            //                            MPI_STATUS_IGNORE); sum_weights += temp_sum_weights;
            //                        }
            //
            //                        for (unsigned long i = 0; i < sum.size(); i++) {
            //                            weighted_mean.at(i) = sum.at(i) / sum_weights;
            //                        }
            //                        return weighted_mean;
            //                    } else {
            //                        MPI_Send(sum.data(), static_cast<int>(sum.size()), MPI_FLOAT, 0, 0,
            //                        MPI_COMM_WORLD); float sum_weights =
            //                        static_cast<float>(std::reduce(weights.begin(), weights.end(), 0.0));
            //                        MPI_Send(&sum_weights, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
            //                        return std::vector<float>(sum.size(), 0);
            //                    }
        }

        [[nodiscard]] float calculate_absolute_mean_error(std::vector<float> absolute_mean) override {
            float sum = 0;

            SphereFunctionState data_feed_state = particle_filter_data_feed->get_state();

            unsigned long mean_index = 0;
            //                    for (unsigned int i = 0; i < data_feed_state.agent_active_status.size(); i++) {
            //                        if (data_feed_state.agent_active_status.at(i) == AgentStatus::active) {
            //                            sum += powf(absolute_mean.at(mean_index) -
            //                            data_feed_state.agents_location.at(i).x, 2); mean_index++; sum +=
            //                            powf(absolute_mean.at(mean_index) - data_feed_state.agents_location.at(i).y,
            //                            2); mean_index++;
            //                        }
            //                    }

            return std::sqrt(sum);
        }

        [[nodiscard]] float calculate_weighted_mean_error(std::vector<float> weighted_mean) override {
            float sum = 0;

            SphereFunctionState data_feed_state = particle_filter_data_feed->get_state();

            //                    unsigned long weighted_mean_index = 0;
            //                    for (unsigned int i = 0; i < data_feed_state.agent_active_status.size(); i++) {
            //                        if (data_feed_state.agent_active_status.at(i) == AgentStatus::active) {
            //                            sum += powf(weighted_mean.at(weighted_mean_index) -
            //                            data_feed_state.agents_location.at(i).x, 2); weighted_mean_index++; sum +=
            //                            powf(weighted_mean.at(weighted_mean_index) -
            //                            data_feed_state.agents_location.at(i).y, 2); weighted_mean_index++;
            //                        }
            //                    }

            return std::sqrt(sum);
        }
    };
} // namespace station_sim

#endif // STATIONSIM_SPHEREFUNCTIONSTATISTICS_HPP
