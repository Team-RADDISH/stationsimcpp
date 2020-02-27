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
#include "Agent.hpp"
#include "Model.hpp"

namespace station_sim {

	class STATIONSIM_EXPORT MultipleModelsRun {
	private:
		std::vector<Model> models;

	public:
		MultipleModelsRun();
		virtual ~MultipleModelsRun();

		virtual void run_all_models();
		void run_model(int model_index);

		void add_model_and_model_parameters(Model model);

		[[nodiscard]] Model& get_model(int index);

		[[nodiscard]] int get_size_of_models_vector();

		[[nodiscard]] bool has_any_model_active();
	};
}

#endif
