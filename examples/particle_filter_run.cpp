//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include <memory>
#include "ModelParameters.hpp"
#include "Model.hpp"
#include "ModelPlotting.hpp"
#include "Timer.hpp"
#include "ParticleFilterParameters.hpp"
#include "ParticleFilter.hpp"

int main()
{
    std::shared_ptr<station_sim::ModelParameters> model_parameters(new station_sim::ModelParameters);
    model_parameters->set_population_total(100);
    model_parameters->set_do_print(false);
    station_sim::Model base_model(0, model_parameters);

    station_sim::ParticleFilter particle_filter(base_model);
    particle_filter.step();

    return 0;
}