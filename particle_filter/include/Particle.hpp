//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef PARTICLE_FILTER_PARTICLE_HPP
#define PARTICLE_FILTER_PARTICLE_HPP

#include <vector>

namespace particle_filter {
    template <class StateType>
    class Particle {
      public:
        Particle() = default;
        virtual ~Particle() = default;

        [[nodiscard]] virtual bool is_active() const = 0;
        [[nodiscard]] virtual const StateType get_state() const = 0;
        virtual void perturb_state(float standard_deviation) = 0;
        virtual void step() = 0;
    };
} // namespace particle_filter

#endif // PARTICLE_FILTER_PARTICLE_HPP