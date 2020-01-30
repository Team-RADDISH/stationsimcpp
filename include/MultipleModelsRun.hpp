//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_MULTIPLEMODELSRUN_HPP
#define STATIONSIM_MULTIPLEMODELSRUN_HPP

#include "stationsim_export.h"
#include <vector>
#include "ModelParameters.hpp"
#include "Model.hpp"

namespace station_sim {

	class STATIONSIM_EXPORT MultipleModelsRun {
	public:
		std::vector<Model> models;
		std::vector<ModelParameters> model_parameters;

	public:
		MultipleModelsRun();
		~MultipleModelsRun();

		void run_all_models();
		void run_model(int model_index);
	};
}

#endif
