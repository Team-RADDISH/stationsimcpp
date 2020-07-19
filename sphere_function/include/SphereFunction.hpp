//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_SphereFunction_HPP
#define STATIONSIM_SphereFunction_HPP

#include "Particle.hpp"
#include "SphereFunctionState.hpp"
#include <array>
#include <memory>
#include <random>
#include <vector>

namespace station_sim {

    class SphereFunction : public Particle<SphereFunctionState> {
      public:
        float x;
        float y;

        std::shared_ptr<std::mt19937> random_number_generator;

      public:
        SphereFunction();
        SphereFunction(float x, float y);
        SphereFunction(const SphereFunction &sphere_function);
        ~SphereFunction() override;

        void reseed_random_number_generator();

        [[nodiscard]] const SphereFunctionState get_state() const override;
        void set_state(const SphereFunctionState &new_state);
        void perturb_state(float standard_deviation) override;

        [[nodiscard]] bool is_active() const override;

        void step() override;
    };
} // namespace station_sim

#endif