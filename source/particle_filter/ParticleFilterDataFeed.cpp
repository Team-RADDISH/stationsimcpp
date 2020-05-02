//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include <particle_filter/ParticleFilterDataFeed.hpp>

namespace station_sim {

    ParticleFilterDateFeed::ParticleFilterDateFeed() {
        ModelParameters model_parameters = ModelParameters();
        model_parameters.set_population_total(40);
        model_parameters.set_do_print(false);
        base_model = station_sim::Model(0, model_parameters);
    }

    std::vector<float> ParticleFilterDateFeed::get_data() {
        base_model.step();
        return base_model.get_state();
    }
} // namespace station_sim
