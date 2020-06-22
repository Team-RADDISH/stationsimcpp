//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include "model/Agent.hpp"
#include "HelpFunctions.hpp"
#include "model/Model.hpp"

#include <algorithm>
#include <cmath>
#include <random>

namespace station_sim {

    Agent::Agent(int unique_id, const Model &model, const ModelParameters &model_parameters,
                 std::shared_ptr<std::mt19937> generator) {
        status = AgentStatus::not_started; // 0 Not Started, 1 Active, 2 Finished
        agent_id = unique_id;

        this->random_number_generator = generator;

        initialize_random_distributions(model_parameters);

        initialize_location(model, model_parameters);
        initialize_speed(model, model_parameters);

        steps_activate = gates_speed_exponential_distribution(*generator);
        wiggle = std::fmin(model_parameters.get_max_wiggle(), agent_max_speed);
    }

    Agent::Agent(const Agent &agent) {
        random_number_generator = agent.random_number_generator;

        float_distribution = agent.float_distribution;
        gates_in_int_distribution = agent.gates_in_int_distribution;
        gates_out_int_distribution = agent.gates_out_int_distribution;
        gates_speed_exponential_distribution = agent.gates_speed_exponential_distribution;
        speed_normal_distribution = agent.speed_normal_distribution;
        wiggle_int_distribution = agent.wiggle_int_distribution;

        agent_id = agent.agent_id;

        steps_activate = agent.steps_activate;
        wiggle = agent.wiggle;
        agent_max_speed = agent.agent_max_speed;
        agent_available_speeds = agent.agent_available_speeds;
        agent_speed = agent.agent_speed;
        history_locations = agent.history_locations;

        history_speeds = agent.history_speeds;
        history_wiggles = agent.history_wiggles;
        history_collisions = agent.history_collisions;
        step_start = agent.step_start;
        status = agent.status;

        steps_taken = agent.steps_taken;
        gate_in = agent.gate_in;
        gate_out = agent.gate_out;
        start_location = agent.start_location;

        desired_location = agent.desired_location;
        agent_location = agent.agent_location;
        steps_delay = agent.steps_delay;
    }

    void Agent::initialize_random_distributions(const ModelParameters &model_parameters) {
        float_distribution = std::uniform_real_distribution<float>(-1, 1);
        gates_in_int_distribution = std::uniform_int_distribution<int>(0, model_parameters.get_gates_in() - 1);
        gates_out_int_distribution = std::uniform_int_distribution<int>(0, model_parameters.get_gates_out() - 1);
        gates_speed_exponential_distribution = std::exponential_distribution<float>(model_parameters.get_gates_speed());
        speed_normal_distribution =
            std::normal_distribution<float>(model_parameters.get_speed_mean(), model_parameters.get_speed_std());
        wiggle_int_distribution = std::uniform_int_distribution<int>(-1, 1);
    }

    void Agent::initialize_location(const Model &model, const ModelParameters &model_parameters) {
        float perturb = float_distribution(*random_number_generator) * model_parameters.get_gates_space();

        gate_in = gates_in_int_distribution(*random_number_generator);
        start_location.x = model.get_gates_in_locations()[gate_in].x;
        start_location.y = model.get_gates_in_locations()[gate_in].y;
        start_location.y += perturb;

        gate_out = gates_out_int_distribution(*random_number_generator);
        desired_location.x = model.get_gates_out_locations()[gate_out].x;
        desired_location.y = model.get_gates_out_locations()[gate_out].y;

        agent_location = start_location;
    }

    void Agent::initialize_speed(const Model &model, const ModelParameters &model_parameters) {
        agent_speed = 0;
        agent_max_speed = 0;

        while (agent_max_speed <= model_parameters.get_speed_min()) {
            agent_max_speed = speed_normal_distribution(*random_number_generator);
        }

        agent_available_speeds = HelpFunctions::evenly_spaced_values_within_interval(
            agent_max_speed, model_parameters.get_speed_min(), -model.get_speed_step());
    }

    void Agent::step(Model &model, const ModelParameters &model_parameters) {
        if (status == AgentStatus::not_started) {
            activate_agent(model);
            move_agent(model, model_parameters);
        } else if (status == AgentStatus::active) {
            move_agent(model, model_parameters);
            deactivate_agent_if_reached_exit_gate(model, model_parameters);
        }

        if (model_parameters.is_do_history()) {
            add_agent_location_history();
        }
    }

    void Agent::activate_agent(Model &model) {
        status = AgentStatus::active;
        model.pop_active += 1;
        step_start = model.step_id;
    }

    void Agent::move_agent(Model &model, const ModelParameters &model_parameters) {
        Point2D direction = calculate_agent_direction();
        Point2D new_agent_location(0, 0);
        float new_speed = 0;

        for (const auto &speed : agent_available_speeds) {
            new_speed = speed;
            new_agent_location.x = agent_location.x + speed * direction.x;
            new_agent_location.y = agent_location.y + speed * direction.y;

            if (is_outside_boundaries(model.boundaries, new_agent_location) ||
                collides_other_agent(model, model_parameters, new_agent_location)) {
                if (model_parameters.is_do_history()) {
                    history_collisions += 1;
                    model.add_to_history_collision_locations(new_agent_location);
                }
            } else {
                break;
            }

            // If even the slowest speed results in a collision, then wiggle.
            if (speed == agent_available_speeds.back()) {
                new_agent_location.x = agent_location.x;
                new_agent_location.y = agent_location.y + wiggle * wiggle_int_distribution(*random_number_generator);

                if (model_parameters.is_do_history()) {
                    history_wiggles += 1;
                    model.add_to_history_wiggle_locations(new_agent_location);
                }
            }
        }

        if (is_outside_boundaries(model.boundaries, new_agent_location)) {
            clip_vector_values_to_boundaries(new_agent_location, model.boundaries);
        }

        agent_location = new_agent_location;
        agent_speed = new_speed;
    }

    void Agent::clip_vector_values_to_boundaries(Point2D &location, std::array<Point2D, 2> boundaries) {
        if (location.x < boundaries[0].x) {
            location.x = boundaries[0].x;
        }
        if (location.x > boundaries[1].x) {
            location.x = boundaries[1].x;
        }

        if (location.y < boundaries[0].y) {
            location.y = boundaries[0].y;
        }
        if (location.y > boundaries[1].y) {
            location.y = boundaries[1].y;
        }
    }

    Point2D Agent::calculate_agent_direction() {
        float distance = calculate_distance(desired_location, agent_location);

        return Point2D((desired_location.x - agent_location.x) / distance,
                       (desired_location.y - agent_location.y) / distance);
    }

    float Agent::calculate_distance(Point2D location_0, Point2D location_1) {
        float sum = powf(location_0.x - location_1.x, 2);
        sum += powf(location_0.y - location_1.y, 2);

        return std::sqrt(sum);
    }

    bool Agent::is_outside_boundaries(const std::array<Point2D, 2> boundaries, const Point2D &location) {
        return location.x < boundaries[0].x || location.x > boundaries[1].x || location.y < boundaries[0].y ||
               location.y > boundaries[1].y;
    }

    bool Agent::collides_other_agent(const Model &model, const ModelParameters &model_parameters,
                                     const Point2D &location) const {
        for (const auto &agent : model.agents) {
            if (agent.agent_id != agent_id && agent.status == AgentStatus::active &&
                calculate_distance(agent.get_agent_location(), location) <= model_parameters.get_separation() &&
                location.x <= agent.get_agent_location().x) {
                return true;
            }
        }
        return false;
    }

    void Agent::deactivate_agent_if_reached_exit_gate(Model &model, const ModelParameters &model_parameters) {
        if (calculate_distance(agent_location, desired_location) < model_parameters.get_gates_space()) {
            status = AgentStatus::finished;
            model.pop_active -= 1;
            model.pop_finished += 1;

            if (model_parameters.is_do_history()) {
                float steps_expected =
                    (calculate_distance(start_location, desired_location) - model_parameters.get_gates_space()) /
                    agent_available_speeds[0];
                model.steps_expected.push_back(steps_expected);
                steps_taken = model.step_id - step_start;
                model.steps_taken.push_back(steps_taken);
                steps_delay = steps_taken - steps_expected;
                model.steps_delay.push_back(steps_delay);
            }
        }
    }

    const Point2D &Agent::get_agent_location() const { return agent_location; }

    float Agent::get_agent_speed() const { return agent_speed; }

    int Agent::get_history_wiggles() const { return history_wiggles; }

    int Agent::get_history_collisions() const { return history_collisions; }

    const std::vector<Point2D> &Agent::get_history_locations() const { return history_locations; }

    void Agent::add_agent_location_history() { history_locations.push_back(agent_location); }

    int Agent::get_agent_id() const { return agent_id; }

    void Agent::set_agent_location(const Point2D &agent_location) { this->agent_location = agent_location; }

    AgentStatus Agent::getStatus() const { return status; }

    void Agent::set_status(AgentStatus status) { this->status = status; }

    void Agent::set_random_number_generator(const std::shared_ptr<std::mt19937> &random_number_generator) {
        Agent::random_number_generator = random_number_generator;
    }

    void Agent::set_desired_location(const Point2D &desired_location) { this->desired_location = desired_location; }

    const Point2D &Agent::get_desired_location() const { return desired_location; }

} // namespace station_sim