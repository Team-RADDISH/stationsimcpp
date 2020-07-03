//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef PARTICLE_FILTER_PARTICLESTATE_HPP
#define PARTICLE_FILTER_PARTICLESTATE_HPP

namespace particle_filter {
    class ParticleState {
      public:
        ParticleState() = default;
        virtual ~ParticleState() = default;

        virtual void mpi_send_state(int rank_destination){};
        virtual void mpi_receive_state(int rank_source){};
    };
} // namespace particle_filter

#endif // PARTICLE_FILTER_PARTICLESTATE_HPP
