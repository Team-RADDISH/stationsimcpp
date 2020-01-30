//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include "MultipleModelsRunMPI.hpp"
#include <iostream>

namespace station_sim {

	MultipleModelsRunMPI::MultipleModelsRunMPI()
	{
		MPI_Init(NULL, NULL);
		MPI_Comm_size(MPI_COMM_WORLD, &mpi_world_size);
		MPI_Comm_rank(MPI_COMM_WORLD, &mpi_world_rank);
	}

	void MultipleModelsRunMPI::run_all_models()
	{
		int chunk_size = ceil(float(get_number_of_models())/float(mpi_world_size));
		int start_chuck = chunk_size*mpi_world_rank;
		int end_chuck = std::min(chunk_size*mpi_world_rank+chunk_size, get_number_of_models());

		std::cout << "world rank: " << mpi_world_rank << " start " << start_chuck << " end " << end_chuck << std::endl;

		for (int i = start_chuck; i<end_chuck; i++) {
			run_model(i);
		}
	}

	MultipleModelsRunMPI::~MultipleModelsRunMPI()
	{
		MPI_Finalize();
	}

	int MultipleModelsRunMPI::get_mpi_world_size() const
	{
		return mpi_world_size;
	}

	int MultipleModelsRunMPI::get_mpi_world_rank() const
	{
		return mpi_world_rank;
	}
}
