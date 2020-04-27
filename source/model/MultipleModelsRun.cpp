//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include "model/MultipleModelsRun.hpp"
#include <algorithm>

namespace station_sim {

    MultipleModelsRun::MultipleModelsRun() = default;

    MultipleModelsRun::~MultipleModelsRun() = default;

    void MultipleModelsRun::run_all_models() {
#pragma omp parallel for schedule(static, 1) default(none)
        for (int i = 0; i < models.size(); i++) {
            run_model(i);
        }
    }

    void MultipleModelsRun::run_model(int model_index) {
        for (int i = 0; i < models[model_index].get_model_parameters().get_step_limit(); i++) {
            models[model_index].step();
        }
    }

    void MultipleModelsRun::add_model_and_model_parameters(Model model) { models.push_back(model); }

    Model &MultipleModelsRun::get_model(int index) { return models[index]; }

    int MultipleModelsRun::get_size_of_models_vector() { return models.size(); }

    bool MultipleModelsRun::has_any_model_active() {
        return std::any_of(models.cbegin(), models.cend(),
                           [](Model model) { return model.get_status() != ModelStatus::finished; });
    }
} // namespace station_sim
