//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_MODELPARAMETERS_CPP
#define STATIONSIM_MODELPARAMETERS_CPP

#include "ModelParameters.hpp"
#include <random>
#include <stdexcept>

namespace station_sim {
    ModelParameters::ModelParameters() {
        this->population_total = 40;

        this->space_width = 200;
        this->space_height = 100;

        this->gates_in_count = 3;
        this->gates_out_count = 2;
        this->gates_space = 1;
        this->gates_speed = 1;

        this->speed_min = static_cast<float>(0.2);
        this->speed_mean = 1;
        this->speed_std = 1;
        this->speed_steps = 3;

        this->separation = 2;
        this->max_wiggle = 1;

        this->step_limit = 3600;

        this->do_history = true;
        this->do_print = true;

        std::random_device r;
        this->random_seed = static_cast<int>(r());
    }

    ModelParameters::~ModelParameters() = default;

    int ModelParameters::get_population_total() const { return population_total; }

    void ModelParameters::set_population_total(int value) {
        if (value <= 0) {
            throw std::invalid_argument("population_total must be positive!");
        }

        this->population_total = value;
    }

    float ModelParameters::get_space_width() const { return space_width; }

    void ModelParameters::set_space_width(float value) {
        if (value <= 0) {
            throw std::invalid_argument("space_width must be positive!");
        }

        this->space_width = value;
    }

    float ModelParameters::get_space_height() const { return space_height; }

    void ModelParameters::set_space_height(float value) {
        if (value <= 0) {
            throw std::invalid_argument("space_height must be positive!");
        }

        this->space_height = value;
    }

    int ModelParameters::get_gates_in() const { return gates_in_count; }

    void ModelParameters::set_gates_in(int value) {
        if (value <= 0) {
            throw std::invalid_argument("gates_in_count must be positive!");
        }

        this->gates_in_count = value;
    }

    int ModelParameters::get_gates_out() const { return gates_out_count; }

    void ModelParameters::set_gates_out(int value) {
        if (value <= 0) {
            throw std::invalid_argument("gates_out_count must be positive!");
        }

        ModelParameters::gates_out_count = value;
    }

    float ModelParameters::get_gates_space() const { return gates_space; }

    void ModelParameters::set_gates_space(float value) {
        if (value <= 0) {
            throw std::invalid_argument("gates_space must be positive!");
        }

        ModelParameters::gates_space = value;
    }

    float ModelParameters::get_gates_speed() const { return gates_speed; }

    void ModelParameters::set_gates_speed(float value) {
        if (value <= 0) {
            throw std::invalid_argument("gates_speed must be positive!");
        }

        ModelParameters::gates_speed = value;
    }

    float ModelParameters::get_speed_min() const { return speed_min; }

    void ModelParameters::set_speed_min(float value) {
        if (value <= 0) {
            throw std::invalid_argument("speed_min must be positive!");
        }

        ModelParameters::speed_min = value;
    }

    float ModelParameters::get_speed_mean() const { return speed_mean; }

    void ModelParameters::set_speed_mean(float value) {
        if (value <= 0) {
            throw std::invalid_argument("speed_mean must be positive!");
        }

        this->speed_mean = value;
    }

    float ModelParameters::get_speed_std() const { return speed_std; }

    void ModelParameters::set_speed_std(float value) {
        if (value <= 0) {
            throw std::invalid_argument("speed_std must be positive!");
        }

        this->speed_std = value;
    }

    int ModelParameters::get_speed_steps() const { return speed_steps; }

    void ModelParameters::set_speed_steps(int value) {
        if (value <= 0) {
            throw std::invalid_argument("speed_steps must be positive!");
        }

        this->speed_steps = value;
    }

    float ModelParameters::get_separation() const { return separation; }

    void ModelParameters::set_separation(float value) {
        if (value <= 0) {
            throw std::invalid_argument("separation must be positive!");
        }

        this->separation = value;
    }

    float ModelParameters::get_max_wiggle() const { return max_wiggle; }

    void ModelParameters::set_max_wiggle(float value) {
        if (value <= 0) {
            throw std::invalid_argument("max_wiggle must be positive!");
        }

        this->max_wiggle = value;
    }

    int ModelParameters::get_step_limit() const { return step_limit; }

    void ModelParameters::set_step_limit(int value) {
        if (value <= 0) {
            throw std::invalid_argument("step_limit must be positive!");
        }

        this->step_limit = value;
    }

    bool ModelParameters::is_do_history() const { return do_history; }

    void ModelParameters::set_do_history(bool value) { this->do_history = value; }

    bool ModelParameters::is_do_print() const { return do_print; }

    void ModelParameters::set_do_print(bool value) { this->do_print = value; }

    int ModelParameters::get_random_seed() const { return random_seed; }

    void ModelParameters::set_random_seed(int value) { ModelParameters::random_seed = value; }
} // namespace station_sim

#endif // STATIONSIM_MODELPARAMETERS_CPP
