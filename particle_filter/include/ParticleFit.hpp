//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//
#ifndef STATIONSIM_PARTICLEFIT_HPP
#define STATIONSIM_PARTICLEFIT_HPP



namespace station_sim {
    template <class ParticleType, class StateType>
    class  ParticleFit {
      public:
        ParticleFit() = default;
        virtual ~ParticleFit() = default;

        [[nodiscard]] virtual float calculate_particle_fit(const ParticleType &particle,
                                                           const StateType &measured_state) const = 0;
    };
} // namespace station_sim

#endif // STATIONSIM_PARTICLEFIT_HPP
