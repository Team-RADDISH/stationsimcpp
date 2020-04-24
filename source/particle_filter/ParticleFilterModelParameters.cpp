//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include "particle_filter/ParticleFilterModelParameters.hpp"

namespace station_sim {

    ParticleFilterModelParameters::ParticleFilterModelParameters() {
        width = 200;
        height = 100;
        pop_total = 100;
        speed_min = 1;
        separation = 2;
        batch_iterations = 4000;
        do_history = false;
        do_print = false;
    }

    int ParticleFilterModelParameters::get_width() const { return width; }

    void ParticleFilterModelParameters::set_width(int width) { this->width = width; }

    int ParticleFilterModelParameters::get_height() const { return height; }

    void ParticleFilterModelParameters::set_height(int height) { this->height = height; }

    int ParticleFilterModelParameters::get_pop_total() const { return pop_total; }

    void ParticleFilterModelParameters::set_pop_total(int pop_total) { this->pop_total = pop_total; }

    int ParticleFilterModelParameters::get_speed_min() const { return speed_min; }

    void ParticleFilterModelParameters::set_speed_min(int speed_min) { this->speed_min = speed_min; }

    float ParticleFilterModelParameters::get_separation() const { return separation; }

    void ParticleFilterModelParameters::set_separation(float separation) { this->separation = separation; }

    int ParticleFilterModelParameters::get_batch_iterations() const { return batch_iterations; }

    void ParticleFilterModelParameters::set_batch_iterations(int batch_iterations) {
        this->batch_iterations = batch_iterations;
    }

    bool ParticleFilterModelParameters::is_do_history() const { return do_history; }

    void ParticleFilterModelParameters::set_do_history(bool do_history) { this->do_history = do_history; }

    bool ParticleFilterModelParameters::is_do_print() const { return do_print; }

    void ParticleFilterModelParameters::set_do_print(bool do_print) { this->do_print = do_print; }

    ParticleFilterModelParameters::~ParticleFilterModelParameters() = default;
} // namespace station_sim
