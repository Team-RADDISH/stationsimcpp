//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include "ParticleFilterParameters.hpp"

namespace station_sim {

    ParticleFilterParameters::ParticleFilterParameters()
    {
        this->number_of_particles = 100;  //todo change that param_list[int(sys.argv[1]) - 1][0],  # particles read from ARC task array variable
        this->number_of_runs = 20;
        this->resample_window = 100;
        this->multi_step = true;
        this->particle_std = 0.5;   //todo change that param_list[int(sys.argv[1]) - 1][2], # Particle noise read from task array variable
        this->model_std = 1.0;
        this->agents_to_visualise = 10;
        this->do_save = true;
        this->plot_save = false;
        this->do_ani = false;
        this->show_ani = false;
    }

    ParticleFilterParameters::~ParticleFilterParameters() = default;

    int ParticleFilterParameters::get_number_of_particles() const
    {
        return number_of_particles;
    }

    void ParticleFilterParameters::set_number_of_particles(int number_of_particles)
    {
        this->number_of_particles = number_of_particles;
    }

    int ParticleFilterParameters::get_number_of_runs() const
    {
        return number_of_runs;
    }

    void ParticleFilterParameters::set_number_of_runs(int number_of_runs)
    {
        this->number_of_runs = number_of_runs;
    }

    int ParticleFilterParameters::get_resample_window() const
    {
        return resample_window;
    }

    void ParticleFilterParameters::set_resample_window(int resample_window)
    {
        this->resample_window = resample_window;
    }

    bool ParticleFilterParameters::is_multi_step() const
    {
        return multi_step;
    }

    void ParticleFilterParameters::set_multi_step(bool multi_step)
    {
        this->multi_step = multi_step;
    }

    float ParticleFilterParameters::get_particle_std() const
    {
        return particle_std;
    }

    void ParticleFilterParameters::set_particle_std(float particle_std)
    {
        this->particle_std = particle_std;
    }

    float ParticleFilterParameters::get_model_std() const
    {
        return model_std;
    }

    void ParticleFilterParameters::set_model_std(float model_std)
    {
        this->model_std = model_std;
    }

    int ParticleFilterParameters::get_agents_to_visualise() const
    {
        return agents_to_visualise;
    }

    void ParticleFilterParameters::set_agents_to_visualise(int agents_to_visualise)
    {
        this->agents_to_visualise = agents_to_visualise;
    }

    bool ParticleFilterParameters::is_do_save() const
    {
        return do_save;
    }

    void ParticleFilterParameters::set_do_save(bool do_save)
    {
        this->do_save = do_save;
    }

    bool ParticleFilterParameters::is_plot_save() const
    {
        return plot_save;
    }

    void ParticleFilterParameters::set_plot_save(bool plot_save)
    {
        this->plot_save = plot_save;
    }

    bool ParticleFilterParameters::is_do_ani() const
    {
        return do_ani;
    }

    void ParticleFilterParameters::set_do_ani(bool do_ani)
    {
        this->do_ani = do_ani;
    }

    bool ParticleFilterParameters::is_show_ani() const
    {
        return show_ani;
    }

    void ParticleFilterParameters::set_show_ani(bool show_ani)
    {
        this->show_ani = show_ani;
    }
}

