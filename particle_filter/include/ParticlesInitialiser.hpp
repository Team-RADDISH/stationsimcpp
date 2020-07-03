//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_PARTICLESINITIALISER_HPP
#define STATIONSIM_PARTICLESINITIALISER_HPP

#include <memory>
#include <vector>

namespace station_sim {
    template <class ParticleType>
    class ParticlesInitialiser {
      private:
      public:
        ParticlesInitialiser() = default;
        virtual ~ParticlesInitialiser() = default;

        [[nodiscard]] virtual std::shared_ptr<std::vector<ParticleType>> initialise_particles(int number_of_particles) const = 0;
    };
} // namespace station_sim

#endif // STATIONSIM_PARTICLESINITIALISER_HPP
