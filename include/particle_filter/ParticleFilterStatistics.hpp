//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_PARTICLEFILTERSTATISTICS_HPP
#define STATIONSIM_PARTICLEFILTERSTATISTICS_HPP

#include "model/Agent.hpp"
#include "model/Model.hpp"
#include "particle_filter/ParticleFilterStatistics.hpp"
#include "stationsim_export.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

namespace station_sim {
    template <class ParticleType, class StateType>
    class STATIONSIM_EXPORT ParticleFilterStatistics {
      private:
        std::shared_ptr<ParticleFilterDataFeed<StateType>> particle_filter_data_feed;

        std::vector<std::vector<float>> weighted_means_states;
        std::vector<float> weighted_mean_errors;
        std::vector<float> absolute_mean_errors;
        std::vector<float> active_agents;

      public:
        ParticleFilterStatistics() = default;
        ~ParticleFilterStatistics() = default;

        void calculate_statistics(const std::shared_ptr<ParticleFilterDataFeed<StateType>> particle_filter_data_feed,
                                  const std::vector<ParticleType> &particles,
                                  const std::vector<float> &particles_weights) {

            std::cout << "Particle filter general statistics:" << std::endl;

            this->particle_filter_data_feed = particle_filter_data_feed;

            std::vector<std::vector<float>> particles_states;
            for (const Particle &particle : particles) {
                particles_states.push_back(particle.get_state());
            }

            std::vector<float> unweighted_mean = calculate_unweighted_average(particles_states);
            std::vector<float> weighted_mean = calculate_weighted_average(particles_states, particles_weights);

            this->weighted_means_states.push_back(weighted_mean);

            this->weighted_mean_errors.push_back(calculate_weighted_mean_error(weighted_mean));
        }

        float calculate_weighted_mean_error(std::vector<float> weighted_mean) {
            float sum = 0;
            for (unsigned int i = 0; i < weighted_mean.size(); i++) {
                sum += powf(weighted_mean[i] - particle_filter_data_feed->get_state()[i], 2);
            }
            return std::sqrt(sum);
        }

        std::vector<float> calculate_unweighted_average(const std::vector<std::vector<float>> &inputs) {
            std::vector<float> unweighted_mean(static_cast<unsigned long>(inputs[0].size()));
            std::vector<float> sum(static_cast<unsigned long>(inputs[0].size()));
            std::fill(sum.begin(), sum.end(), 0);

            for (const std::vector<float> &input : inputs) {
                for (unsigned long j = 0; j < input.size(); j++) {
                    sum[j] = sum[j] + input[j];
                }
            }

            for (unsigned long i = 0; i < unweighted_mean.size(); i++) {
                unweighted_mean[i] = sum[i] / static_cast<float>(inputs[0].size());
            }

            return unweighted_mean;
        }

        std::vector<float> calculate_weighted_average(const std::vector<std::vector<float>> &inputs,
                                                      const std::vector<float> &weights) {

            std::vector<float> weighted_mean(static_cast<unsigned long>(inputs[0].size()));

            std::vector<float> sum(static_cast<unsigned long>(inputs[0].size()));
            std::fill(sum.begin(), sum.end(), 0);

            for (const std::vector<float> &input : inputs) {
                for (unsigned long j = 0; j < input.size(); j++) {
                    sum[j] = sum[j] + input[j];
                }
            }

            float sum_weights = static_cast<float>(std::accumulate(weights.begin(), weights.end(), 0.0));

            for (unsigned long i = 0; i < weighted_mean.size(); i++) {
                weighted_mean[i] = sum[i] * weights[i] / sum_weights;
            }

            return weighted_mean;
        }

        //        std::vector<float> calculate_variance(const Model &base_model, const std::vector<Model> &models,
        //                                              const std::vector<float> &weights,
        //                                              const std::vector<bool> &base_model_agents_status,
        //                                              const long base_model_active_agents,
        //                                              const std::vector<float> &unweighted_mean) {
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

        const std::vector<float> &get_active_agents() const { return active_agents; }
        const std::vector<std::vector<float>> &get_weighted_means_states() const { return weighted_means_states; }
        const std::vector<float> &get_weighted_mean_errors() const { return weighted_mean_errors; }
        const std::vector<float> &get_absolute_mean_errors() const { return absolute_mean_errors; }
    };
} // namespace station_sim
#endif // STATIONSIM_PARTICLEFILTERSTATISTICS_HPP
