//
// Created by ea461 on 07/01/2020.
//

#include "ModelParameters.hpp"
#include "Model.hpp"

int main()
{
	station_sim::ModelParameters model_parameters;
	station_sim::Model model(0, model_parameters);

	for (int i = 0; i<model_parameters.get_step_limit(); i++) {
		model.step(model, model_parameters);
	}
}