//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_MODELPLOTTING_HPP
#define STATIONSIM_MODELPLOTTING_HPP

#include "Point2D.hpp"
#include <array>
#include <vector>

namespace station_sim {
    class Model;

    class ModelPlotting {
      public:
        ModelPlotting() = delete;

        static void plot_agents_locations(const Model &model);

        static void plot_gates_locations(const std::vector<Point2D> &gates);

        static void plot_agents_trails(const Model &model);
    };
} // namespace station_sim
#endif // STATIONSIM_MODELPLOTTING_HPP
