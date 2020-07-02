//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_PARTICLESTATE_HPP
#define STATIONSIM_PARTICLESTATE_HPP

#include "stationsim_export.h"

namespace station_sim {
    class STATIONSIM_EXPORT ParticleState {
      public:
        ParticleState() = default;
        virtual ~ParticleState() = default;

        virtual void mpi_send_state(int rank_destination){};
        virtual void mpi_receive_state(int rank_source){};
    };
} // namespace station_sim

#endif // STATIONSIM_PARTICLESTATE_HPP
