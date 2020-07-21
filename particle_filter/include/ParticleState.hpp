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

        virtual void mpi_send_state(int rank_destination) = 0;
        virtual void mpi_receive_state(int rank_source) = 0;
    };
} // namespace particle_filter

#endif // PARTICLE_FILTER_PARTICLESTATE_HPP
