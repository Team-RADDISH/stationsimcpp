//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include "SphereFunction.hpp"
#include <algorithm>

namespace station_sim {
    SphereFunction::SphereFunction() {
        reseed_random_number_generator();

        this->x = 0;
        this->y = 0;
    }

    SphereFunction::~SphereFunction() = default;

    SphereFunction::SphereFunction(float x, float y) {
        reseed_random_number_generator();

        this->x = x;
        this->y = y;
    }

    SphereFunction::SphereFunction(const SphereFunction &sphere_function) {
        reseed_random_number_generator();

        this->x = sphere_function.x;
        this->y = sphere_function.y;
    }

    void SphereFunction::reseed_random_number_generator() {
        std::random_device rd;
        std::array<int, std::mt19937::state_size> seed_data;
        std::generate_n(seed_data.data(), seed_data.size(), std::ref(rd));
        std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
        random_number_generator = std::make_shared<std::mt19937>(std::mt19937(seq));
    }

    const SphereFunctionState SphereFunction::get_state() const {
        SphereFunctionState sphere_function_state;

        sphere_function_state.x = x;
        sphere_function_state.y = y;

        return sphere_function_state;
    }

    void SphereFunction::set_state(const SphereFunctionState &new_state) {
        this->x = new_state.x;
        this->y = new_state.y;
    }

    void SphereFunction::perturb_state(float standard_deviation) {
        std::normal_distribution<float> dis(0.0, standard_deviation);

        this->x += dis(*random_number_generator);
        this->y += dis(*random_number_generator);
    }

    bool SphereFunction::is_active() const { return true; }

    void SphereFunction::step(){};
} // namespace station_sim