//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_PARTICLEFILTERDATAFEED_HPP
#define STATIONSIM_PARTICLEFILTERDATAFEED_HPP

#include "stationsim_export.h"
#include <model/Model.hpp>
#include <vector>

namespace station_sim {
    template <class T>
    class STATIONSIM_EXPORT ParticleFilterDataFeed {
      private:
        station_sim::ModelParameters model_parameters;
        station_sim::Model base_model;
        std::mt19937 *generator;
        std::normal_distribution<float> float_normal_distribution;

      public:
        ParticleFilterDataFeed() {
            std::random_device r;
            generator = new std::mt19937(r());
            float target_model_std = 1.0;
            float_normal_distribution = std::normal_distribution<float>(0.0, powf(target_model_std, 2));

            ModelParameters model_parameters = ModelParameters();
            model_parameters.set_population_total(40);
            model_parameters.set_do_print(false);
            base_model = station_sim::Model(0, model_parameters);
        }
        ~ParticleFilterDataFeed() = default;

        [[nodiscard]] T get_data() {
            base_model.step();

            // Add noise to the synthetic target data
            std::vector<Point2D> measured_state(base_model.agents.size());
            for (unsigned long i = 0; i < base_model.agents.size(); i++) {
                Point2D agent_location = base_model.agents[i].get_agent_location();
                measured_state.at(i).x = agent_location.x + float_normal_distribution(*generator);
                measured_state.at(i).y = agent_location.y + float_normal_distribution(*generator);
            }

            return base_model.get_state();
        }
    };
} // namespace station_sim

#endif // STATIONSIM_PARTICLEFILTERDATAFEED_HPP
