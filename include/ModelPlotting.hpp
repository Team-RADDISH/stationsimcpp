//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_MODELPLOTTING_HPP
#define STATIONSIM_MODELPLOTTING_HPP

#include "stationsim_export.h"

namespace station_sim {

	class Model;

	class STATIONSIM_EXPORT ModelPlotting {
	public:
		ModelPlotting() = delete;

		static void plot_agents_locations(const Model& model);

		static void plot_gates_locations(const std::vector<std::array<float, 2>>& gates);

		static void plot_agents_trails(const Model& model);
	};
}
#endif //STATIONSIM_MODELPLOTTING_HPP
