//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_MODEL_HPP
#define STATIONSIM_MODEL_HPP

#include "Point2D.hpp"
#include "model/Agent.hpp"
#include "model/ModelState.hpp"
#include "particle_filter/Particle.hpp"
#include "stationsim_export.h"

#include "H5Cpp.h"
#include <array>
#include <memory>
#include <random>
#include <vector>

namespace station_sim {

    class ModelParameters;

    enum class STATIONSIM_EXPORT ModelStatus : int { not_started = 0, active = 1, finished = 2 };

    class STATIONSIM_EXPORT Model : public Particle<ModelState> {
      private:
        int model_id;
        ModelStatus status;
        float speed_step;

        // History related variables
        int history_collisions_number;
        std::vector<Point2D> history_collision_locations;
        int wiggle_collisions_number;
        std::vector<Point2D> history_wiggle_locations;
        std::shared_ptr<std::mt19937> random_number_generator;
        ModelParameters model_parameters;

        int print_per_steps;
        std::vector<std::vector<Point2D>> history_state;

      public:
        int step_id = 0;
        int pop_active = 0;
        int pop_finished = 0;
        std::array<Point2D, 2> boundaries;
        std::vector<Point2D> gates_in_locations;
        std::vector<Point2D> gates_out_locations;
        std::vector<Agent> agents;
        [[nodiscard]] std::shared_ptr<std::mt19937> get_generator() const;
        std::vector<float> steps_expected;
        std::vector<float> steps_taken;
        std::vector<float> steps_delay;

        Model() = default;
        Model(int unique_id, ModelParameters model_parameters);
        Model(const Model &model);
        ~Model() override;

        [[nodiscard]] int get_unique_id() const;
        [[nodiscard]] const std::vector<Point2D> &get_gates_in_locations() const;
        [[nodiscard]] const std::vector<Point2D> &get_gates_out_locations() const;
        void step();
        [[nodiscard]] int get_history_collisions_number() const;
        void set_history_collisions_number(int history_collisions_number);
        void increase_history_collisions_number_by_value(int value_increase);
        void add_to_history_collision_locations(Point2D new_location);
        void increase_wiggle_collisions_number_by_value(int value_increase);
        void add_to_history_wiggle_locations(Point2D new_location);
        [[nodiscard]] float get_speed_step() const;
        void calculate_print_model_run_analytics();
        [[nodiscard]] ModelParameters get_model_parameters() const;
        [[nodiscard]] bool model_simulation_finished();
        void write_model_output_to_hdf_5(std::string file_name);
        [[nodiscard]] ModelStatus get_status() const;
        void reseed_random_number_generator();
        [[nodiscard]] std::vector<Point2D> get_agents_location();
        [[nodiscard]] const ModelState get_state() const override;
        void set_state(const ModelState &new_state);
        [[nodiscard]] std::vector<float> get_active_agents_state() const;
        [[nodiscard]] bool is_active() const override;

      private:
        void initialize_model(int unique_id);
        void set_boundaries();
        void set_gates_locations();
        static void create_gates(std::vector<Point2D> &gates, float x, float y, int gates_number);
        void generate_agents();
        void move_agents();

        void write_agent_locations_to_hdf_5(H5::Group &history_group);

        void write_model_parameters_to_hdf5(H5::Group &model_parameters_group);

        void write_collisions_history_to_hdf_5(H5::Group &history_group);

        void write_wiggle_history_to_hdf_5(H5::Group &history_group);
    };
} // namespace station_sim

#endif