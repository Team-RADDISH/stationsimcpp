//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include "Model.hpp"
#include "ModelPlotting.hpp"
#include "Agent.hpp"
#include "cxxplot.hpp"

namespace station_sim {
	void ModelPlotting::plot_agents_locations(const station_sim::Model& model)
	{
		std::vector<float> x_int;
		std::vector<float> y_int;

		for (const auto& agent : model.agents) {
			const Point2D agent_location = agent.get_agent_location();
			x_int.emplace_back(agent_location.x);
			y_int.emplace_back(agent_location.y);
		}

		std::vector<std::pair<std::string, std::string>> args;
		args.emplace_back("color", "r");
		args.emplace_back("marker", "o");

		cxxplot::Plot<float> plot(x_int, y_int, args);
		plot.set_xlabel("x label");
		plot.set_ylabel("y label");
		plot.show_plot();
	}

	void ModelPlotting::plot_gates_locations(const std::vector<std::array<float, 2>>& gates)
	{
		std::vector<float> x_int;
		std::vector<float> y_int;

		for (const auto& gate : gates) {
			const std::array<float, 2> agent_location = gate;
			x_int.emplace_back(agent_location[0]);
			y_int.emplace_back(agent_location[1]);
		}

		std::vector<std::pair<std::string, std::string>> args;
		args.emplace_back("color", "r");
		args.emplace_back("marker", "o");

		cxxplot::Plot<float> plot(x_int, y_int, args);
		plot.set_xlabel("x label");
		plot.set_ylabel("y label");
		plot.show_plot();
	}
}