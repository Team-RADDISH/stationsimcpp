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
      protected:
        std::shared_ptr<ParticleFilterDataFeed<StateType>> particle_filter_data_feed;
        std::vector<std::vector<float>> weighted_means_states;
        std::vector<float> weighted_mean_errors;
        std::vector<float> absolute_mean_errors;

      public:
        ParticleFilterStatistics() = default;
        virtual ~ParticleFilterStatistics() = default;

        virtual void
        calculate_statistics(const std::shared_ptr<ParticleFilterDataFeed<StateType>> particle_filter_data_feed,
                             const std::vector<ParticleType> &particles,
                             const std::vector<float> &particles_weights) = 0;

        virtual float calculate_weighted_mean_error(std::vector<float> weighted_mean) = 0;

        virtual std::vector<float> calculate_unweighted_average(const std::vector<StateType> &particles_states) = 0;

        virtual std::vector<float> calculate_weighted_average(const std::vector<StateType> &particles_states,
                                                              const std::vector<float> &weights) = 0;

        //        virtual std::vector<float> calculate_variance(const Model &base_model, const std::vector<Model>
        //        &models,
        //                                              const std::vector<float> &weights,
        //                                              const std::vector<bool> &base_model_agents_status,
        //                                              const long base_model_active_agents,
        //                                              const std::vector<float> &unweighted_mean) = 0;

        [[nodiscard]] const std::vector<std::vector<float>> &get_weighted_means_states() const { return weighted_means_states; }
        [[nodiscard]] const std::vector<float> &get_weighted_mean_errors() const { return weighted_mean_errors; }
        [[nodiscard]] const std::vector<float> &get_absolute_mean_errors() const { return absolute_mean_errors; }
    };
} // namespace station_sim
#endif // STATIONSIM_PARTICLEFILTERSTATISTICS_HPP
