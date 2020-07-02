//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_MODELSTATE_HPP
#define STATIONSIM_MODELSTATE_HPP

#include "array"
#include "model/Point2D.hpp"
#include "mpi.h"
#include "particle_filter/ParticleState.hpp"
#include "stationsim_export.h"
#include <algorithm>
#include <vector>

namespace station_sim {

    class STATIONSIM_EXPORT ModelState : public ParticleState {
      public:
        std::vector<Point2D> agents_location;
        std::vector<AgentStatus> agent_active_status;
        std::vector<Point2D> agents_desired_location;

        ModelState() = default;
        ~ModelState() override = default;

        void mpi_send_state(int rank_destination) override {
            // send agents' locations
            std::vector<float> x(agents_location.size());
            std::vector<float> y(agents_location.size());

            for (unsigned long i = 0; i < agents_location.size(); i++) {
                x.at(i) = agents_location.at(i).x;
                y.at(i) = agents_location.at(i).y;
            }

            MPI_Send(x.data(), static_cast<int>(x.size()), MPI_FLOAT, rank_destination, 0, MPI_COMM_WORLD);
            MPI_Send(y.data(), static_cast<int>(y.size()), MPI_FLOAT, rank_destination, 0, MPI_COMM_WORLD);

            // send agents' status
            std::vector<int> agent_status(agent_active_status.size());
            for (unsigned long i = 0; i < agent_active_status.size(); i++) {
                agent_status.at(i) = static_cast<int>(agent_active_status.at(i));
            }
            MPI_Send(agent_status.data(), static_cast<int>(agent_status.size()), MPI_INT, rank_destination, 0,
                     MPI_COMM_WORLD);

            // send agents' desired locations
            for (unsigned long i = 0; i < agents_desired_location.size(); i++) {
                x.at(i) = agents_desired_location.at(i).x;
                y.at(i) = agents_desired_location.at(i).y;
            }
            MPI_Send(x.data(), static_cast<int>(x.size()), MPI_FLOAT, rank_destination, 0, MPI_COMM_WORLD);
            MPI_Send(y.data(), static_cast<int>(y.size()), MPI_FLOAT, rank_destination, 0, MPI_COMM_WORLD);
        }

        void mpi_receive_state(int rank_source) override {
            // receive agent locations
            std::vector<float> x(agents_location.size());
            std::vector<float> y(agents_location.size());

            MPI_Recv(x.data(), static_cast<int>(x.size()), MPI_FLOAT, rank_source, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            MPI_Recv(y.data(), static_cast<int>(y.size()), MPI_FLOAT, rank_source, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);

            for (unsigned long i = 0; i < agents_location.size(); i++) {
                agents_location.at(i).x = x.at(i);
                agents_location.at(i).y = y.at(i);
            }

            // receive agents' status
            std::vector<int> agent_status(agent_active_status.size());
            MPI_Recv(agent_status.data(), static_cast<int>(agent_status.size()), MPI_INT, rank_source, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (unsigned long i = 0; i < agent_active_status.size(); i++) {
                agent_active_status.at(i) = static_cast<AgentStatus>(agent_status.at(i));
            }

            // receive agents' desired locations
            MPI_Recv(x.data(), static_cast<int>(x.size()), MPI_FLOAT, rank_source, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);
            MPI_Recv(y.data(), static_cast<int>(y.size()), MPI_FLOAT, rank_source, 0, MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);

            for (unsigned long i = 0; i < agents_desired_location.size(); i++) {
                agents_desired_location.at(i).x = x.at(i);
                agents_desired_location.at(i).y = y.at(i);
            }
        }
    };
} // namespace station_sim

#endif // STATIONSIM_MODELSTATE_HPP
