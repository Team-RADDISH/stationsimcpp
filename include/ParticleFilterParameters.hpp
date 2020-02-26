//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_PARTICLEFILTERPARAMETERS_HPP
#define STATIONSIM_PARTICLEFILTERPARAMETERS_HPP

#include "stationsim_export.h"

namespace station_sim {
    class STATIONSIM_EXPORT ParticleFilterParameters {

    private:
        int number_of_particles;
        int number_of_runs;
        int resample_window;
        bool multi_step;
        float particle_std;
        float model_std;
        int agents_to_visualise;
        bool do_save;
        bool plot_save;
        bool do_ani;
        bool show_ani;

    public:
        ParticleFilterParameters();

        ~ParticleFilterParameters();

        [[nodiscard]] int get_number_of_particles() const;

        void set_number_of_particles(int number_of_particles);

        [[nodiscard]] int get_number_of_runs() const;

        void set_number_of_runs(int number_of_runs);

        [[nodiscard]] int get_resample_window() const;

        void set_resample_window(int resample_window);

        [[nodiscard]] bool is_multi_step() const;

        void set_multi_step(bool multi_step);

        [[nodiscard]] float get_particle_std() const;

        void set_particle_std(float particle_std);

        [[nodiscard]] float get_model_std() const;

        void set_model_std(float model_std);

        [[nodiscard]] int get_agents_to_visualise() const;

        void set_agents_to_visualise(int agents_to_visualise);

        [[nodiscard]] bool is_do_save() const;

        void set_do_save(bool do_save);

        [[nodiscard]] bool is_plot_save() const;

        void set_plot_save(bool plot_save);

        [[nodiscard]] bool is_do_ani() const;

        void set_do_ani(bool do_ani);

        [[nodiscard]] bool is_show_ani() const;

        void set_show_ani(bool show_ani);
    };
}

#endif //STATIONSIM_PARTICLEFILTERPARAMETERS_HPP
