//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_MODELPARAMETERS_HPP
#define STATIONSIM_MODELPARAMETERS_HPP

#include "Point2D.hpp"
#include <vector>

namespace station_sim {
    class ModelParameters {
      private:
        int population_total;

        std::vector<Point2D> boundaries;

        int gates_in_count;
        int gates_out_count;
        float gates_space;
        float gates_speed;

        float speed_min;
        float speed_mean;
        float speed_std;
        int speed_steps;

        float separation;
        float max_wiggle;

        int step_limit;

        bool do_history;
        bool do_print;

        int random_seed;

      public:
        ModelParameters();

        ~ModelParameters();

        [[nodiscard]] int get_population_total() const;
        void set_population_total(int value);
        [[nodiscard]] std::vector<Point2D> get_boundaries() const;
        void set_boundaries(std::vector<Point2D> boundaries);
        [[nodiscard]] int get_gates_in() const;
        void set_gates_in(int value);
        [[nodiscard]] int get_gates_out() const;
        void set_gates_out(int value);
        [[nodiscard]] float get_gates_space() const;
        void set_gates_space(float value);
        [[nodiscard]] float get_gates_speed() const;
        void set_gates_speed(float value);
        [[nodiscard]] float get_speed_min() const;
        void set_speed_min(float value);
        [[nodiscard]] float get_speed_mean() const;
        void set_speed_mean(float value);
        [[nodiscard]] float get_speed_std() const;
        void set_speed_std(float value);
        [[nodiscard]] int get_speed_steps() const;
        void set_speed_steps(int value);
        [[nodiscard]] float get_separation() const;
        void set_separation(float value);
        [[nodiscard]] float get_max_wiggle() const;
        void set_max_wiggle(float value);
        [[nodiscard]] int get_step_limit() const;
        void set_step_limit(int value);
        [[nodiscard]] bool is_do_history() const;
        void set_do_history(bool value);
        [[nodiscard]] bool is_do_print() const;
        void set_do_print(bool value);
        [[nodiscard]] int get_random_seed() const;
        void set_random_seed(int value);
    };
} // namespace station_sim

#endif // STATIONSIM_MODELPARAMETERS_HPP
