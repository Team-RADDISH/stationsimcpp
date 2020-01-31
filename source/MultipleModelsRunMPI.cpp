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

	MultipleModelsRunMPI::MultipleModelsRunMPI(int number_of_models)
	{
		MPI_Init(NULL, NULL);
		MPI_Comm_size(MPI_COMM_WORLD, &mpi_world_size);
		MPI_Comm_rank(MPI_COMM_WORLD, &mpi_world_rank);

		this->number_of_models = number_of_models;

		chunk_size = ceil(float(number_of_models)/float(mpi_world_size));
		start_chuck = chunk_size*mpi_world_rank;
		end_chuck = std::min(chunk_size*mpi_world_rank+chunk_size, number_of_models);
	}

	void
	MultipleModelsRunMPI::add_model_and_model_parameters(Model model, ModelParameters model_parameters, int model_index)
	{
		if (model_index>=start_chuck && model_index<end_chuck) {
			MultipleModelsRun::add_model_and_model_parameters(model);
		}
	}

	void MultipleModelsRunMPI::run_all_models()
	{
		std::cout << "world rank: " << mpi_world_rank << " start " << start_chuck << " end " << end_chuck << std::endl;

#pragma omp parallel for schedule(static, 1) default(none)
		for (int i = 0; i<get_size_of_models_vector(); i++) {
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
