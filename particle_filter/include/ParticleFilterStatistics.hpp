//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef PARTICLE_FILTER_PARTICLEFILTERSTATISTICS_HPP
#define PARTICLE_FILTER_PARTICLEFILTERSTATISTICS_HPP

#include "ParticleFilterStatistics.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <vector>

namespace particle_filter {
    template <class ParticleType, class StateType>
    class ParticleFilterStatistics {
      protected:
        std::shared_ptr<ParticleFilterDataFeed<StateType>> particle_filter_data_feed;
        std::vector<std::vector<float>> absolute_means_states;
        std::vector<std::vector<float>> weighted_means_states;
        std::vector<float> weighted_mean_errors;
        std::vector<float> absolute_mean_errors;
        std::vector<long> active_agents;

      public:
        ParticleFilterStatistics() = default;
        virtual ~ParticleFilterStatistics() = default;

        virtual void
        calculate_statistics(const std::shared_ptr<ParticleFilterDataFeed<StateType>> particle_filter_data_feed,
                             const std::vector<ParticleType> &particles,
                             const std::vector<float> &particles_weights) = 0;

        virtual float calculate_absolute_mean_error(std::vector<float> weighted_mean) = 0;
        virtual float calculate_weighted_mean_error(std::vector<float> weighted_mean) = 0;

        virtual std::vector<float> calculate_absolute_average(const std::vector<StateType> &particles_states) = 0;

        virtual std::vector<float> calculate_weighted_average(const std::vector<StateType> &particles_states,
                                                              const std::vector<float> &weights) = 0;

        //        virtual std::vector<float> calculate_variance(const Model &base_model, const std::vector<Model>
        //        &models,
        //                                              const std::vector<float> &weights,
        //                                              const std::vector<bool> &base_model_agents_status,
        //                                              const long base_model_active_agents,
        //                                              const std::vector<float> &unweighted_mean) = 0;

        [[nodiscard]] const std::vector<std::vector<float>> &get_weighted_means_states() const {
            return weighted_means_states;
        }
        [[nodiscard]] const std::vector<float> &get_weighted_mean_errors() const { return weighted_mean_errors; }
        [[nodiscard]] const std::vector<float> &get_absolute_mean_errors() const { return absolute_mean_errors; }
        [[nodiscard]] const std::vector<long> &get_active_agents() const { return active_agents; }
    };
} // namespace particle_filter
#endif // PARTICLE_FILTER_PARTICLEFILTERSTATISTICS_HPP
