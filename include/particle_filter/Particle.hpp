//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_PARTICLE_HPP
#define STATIONSIM_PARTICLE_HPP

#include <memory>

namespace station_sim {
    class Particle {
      private:
      public:
        Particle() = default;
        virtual ~Particle() = default;

        [[nodiscard]] virtual bool is_active() const { return false; };
    };
} // namespace station_sim

#endif // STATIONSIM_PARTICLE_HPP