//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//
#ifndef PARTICLE_FILTER_PARTICLEFIT_HPP
#define PARTICLE_FILTER_PARTICLEFIT_HPP

namespace particle_filter {
    template <class ParticleType, class StateType>
    class ParticleFit {
      public:
        ParticleFit() = default;
        virtual ~ParticleFit() = default;

        [[nodiscard]] virtual float calculate_particle_fit(const ParticleType &particle,
                                                           const StateType &measured_state) const = 0;
    };
} // namespace particle_filter

#endif // PARTICLE_FILTER_PARTICLEFIT_HPP
