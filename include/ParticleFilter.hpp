//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_PARTICLEFILTER_HPP
#define STATIONSIM_PARTICLEFILTER_HPP

#include "stationsim_export.h"
#include "Model.hpp"
#include "ParticleFilterParameters.hpp"
#include "MultipleModelsRun.hpp"

namespace station_sim {
    class STATIONSIM_EXPORT ParticleFilter {
    private:
        int number_of_particles;
        int number_of_runs;
        int resample_window;
        bool multi_step;
        float particle_std;
        float model_std;
        int agents_to_visualise;
        bool do_save;
        bool do_resample;
        bool plot_save;
        bool do_ani;
        bool show_ani;

        int steps_run;
        int number_of_step_to_run;
        int window_counter;

        std::vector<float> weights;

        std::mt19937* generator;
        std::normal_distribution<float> float_normal_distribution;

        Model base_model;
        MultipleModelsRun multiple_models_run;

    public:
        ParticleFilter() = delete;

        explicit ParticleFilter(Model base_model);

        ~ParticleFilter();

        void step();

        void predict(int numiter = 1);

        void step_particle(Model& model, int num_iter, float particle_std);

        void reweight();

        void resample();
    };
}

#endif //STATIONSIM_PARTICLEFILTER_HPP
