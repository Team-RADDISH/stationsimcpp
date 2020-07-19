//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_SPHEREFUNCTIONSTATE_HPP
#define STATIONSIM_SPHEREFUNCTIONSTATE_HPP

#include "ParticleState.hpp"
#include "array"
#include "mpi.h"
#include <algorithm>
#include <vector>

using namespace particle_filter;

namespace station_sim {
    class SphereFunctionState : public ParticleState {
      public:
        float x;
        float y;

        SphereFunctionState() = default;
        ~SphereFunctionState() override = default;

        void mpi_send_state(int rank_destination) override {
            MPI_Send(&x, 1, MPI_FLOAT, rank_destination, 0, MPI_COMM_WORLD);
            MPI_Send(&y, 1, MPI_FLOAT, rank_destination, 0, MPI_COMM_WORLD);
        }

        void mpi_receive_state(int rank_source) override {
            MPI_Recv(&x, 1, MPI_FLOAT, rank_source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&y, 1, MPI_FLOAT, rank_source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    };
} // namespace station_sim

#endif // STATIONSIM_SPHEREFUNCTIONSTATE_HPP
