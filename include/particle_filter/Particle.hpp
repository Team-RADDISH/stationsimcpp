//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_PARTICLE_HPP
#define STATIONSIM_PARTICLE_HPP

#include <vector>

namespace station_sim {
    template<class StateType>
    class Particle {
      public:
        Particle() = default;
        virtual ~Particle() = default;

        [[nodiscard]] virtual bool is_active() const = 0;
        [[nodiscard]] virtual const std::vector<StateType> get_state() const = 0;
    };
} // namespace station_sim

#endif // STATIONSIM_PARTICLE_HPP