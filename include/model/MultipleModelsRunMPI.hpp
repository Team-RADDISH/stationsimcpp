//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_MULTIPLEMODELSRUNMPI_HPP
#define STATIONSIM_MULTIPLEMODELSRUNMPI_HPP

#include "model/Model.hpp"
#include "model/ModelParameters.hpp"
#include "model/MultipleModelsRun.hpp"
#include "mpi.h"
#include "stationsim_export.h"
#include <vector>

namespace station_sim {

    class STATIONSIM_EXPORT MultipleModelsRunMPI : public MultipleModelsRun {
      private:
        int mpi_world_size;
        int mpi_world_rank;
        int number_of_models;

        int chunk_size;
        int start_chuck;
        int end_chuck;

      public:
        explicit MultipleModelsRunMPI(int number_of_models);
        ~MultipleModelsRunMPI();

        void add_model_and_model_parameters(Model model, ModelParameters model_parameters, int model_index);

        void run_all_models() override;

        [[nodiscard]] int get_mpi_world_size() const;
        [[nodiscard]] int get_mpi_world_rank() const;
    };
} // namespace station_sim

#endif
