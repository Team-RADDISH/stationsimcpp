//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_MULTIPLEMODELSRUNMPI_HPP
#define STATIONSIM_MULTIPLEMODELSRUNMPI_HPP

#include "stationsim_export.h"
#include <vector>
#include "MultipleModelsRun.hpp"
#include "ModelParameters.hpp"
#include "Model.hpp"
#include "mpi.h"

namespace station_sim {

	class STATIONSIM_EXPORT MultipleModelsRunMPI : public MultipleModelsRun {
	private:
		int mpi_world_size;
	public:
		[[nodiscard]] int get_mpi_world_size() const;
		[[nodiscard]] int get_mpi_world_rank() const;
	private:
		int mpi_world_rank;

	public:
		MultipleModelsRunMPI();
		~MultipleModelsRunMPI();

		void run_all_models() override;
	};
}

#endif
