//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_AGENT_HPP
#define STATIONSIM_AGENT_HPP

#include "ModelParameters.hpp"
#include "Point2D.hpp"
#include <array>
#include <memory>
#include <random>
#include <vector>

namespace station_sim {
    class Model;
    enum class AgentStatus : int { not_started = 0, active = 1, finished = 2 };

    class Agent {
      public:
        void set_random_number_generator(const std::shared_ptr<std::mt19937> &random_number_generator);
        const Point2D &get_desired_location() const;
        void set_desired_location(const Point2D &desired_location);

      private:
        std::shared_ptr<std::mt19937> random_number_generator;
        std::uniform_real_distribution<float> float_distribution;
        std::uniform_int_distribution<int> gates_in_int_distribution;
        std::uniform_int_distribution<int> gates_out_int_distribution;
        std::exponential_distribution<float> gates_speed_exponential_distribution;
        std::normal_distribution<float> speed_normal_distribution;
        std::uniform_int_distribution<int> wiggle_int_distribution;

        int agent_id;
        int steps_activate;
        float wiggle;
        float agent_max_speed;
        std::vector<float> agent_available_speeds;
        float agent_speed;
        std::vector<Point2D> history_locations;

        std::vector<float> history_speeds;
        int history_wiggles = 0;
        int history_collisions = 0;
        int step_start{};
        AgentStatus status;

        int steps_taken;
        int gate_in;
        Point2D start_location;
        int gate_out;
        Point2D desired_location;
        Point2D agent_location;
        float steps_delay;

      public:
        Agent() = delete;
        Agent(int unique_id, const Model &model, const ModelParameters &model_parameters,
              std::shared_ptr<std::mt19937> generator);
        Agent(const Agent &agent);
        ~Agent() = default;

        void step(Model &model, const ModelParameters &model_parameters);
        [[nodiscard]] const Point2D &get_agent_location() const;
        [[nodiscard]] float get_agent_speed() const;
        [[nodiscard]] const std::vector<Point2D> &get_history_locations() const;
        [[nodiscard]] int get_history_wiggles() const;
        [[nodiscard]] int get_history_collisions() const;
        [[nodiscard]] AgentStatus getStatus() const;
        void set_status(AgentStatus status);
        [[nodiscard]] int get_agent_id() const;
        void set_agent_location(const Point2D &agent_location);
        [[nodiscard]] static bool is_outside_boundaries(std::vector<Point2D> boundaries, const Point2D &location);


      private:
        void initialize_location(const Model &model, const ModelParameters &model_parameters);
        void initialize_speed(const Model &model, const ModelParameters &model_parameters);
        void activate_agent(Model &model);
        void deactivate_agent_if_reached_exit_gate(Model &model, const ModelParameters &model_parameters);
        [[nodiscard]] Point2D calculate_agent_direction();
        [[nodiscard]] bool collides_other_agent(const Model &model, const ModelParameters &model_parameters,
                                                const Point2D &location) const;
        static void clip_vector_values_to_boundaries(Point2D &location, std::vector<Point2D> boundaries);
        void initialize_random_distributions(const ModelParameters &model_parameters);

        void move_agent(Model &model, const ModelParameters &model_parameters);

        void add_agent_location_history();
    };
} // namespace station_sim
#endif // STATIONSIM_AGENT_HPP
