//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include <algorithm>
#include <iostream>
#include <numeric>

#include "HelpFunctions.hpp"
#include "model/Agent.hpp"
#include "model/Model.hpp"
#include "model/ModelParameters.hpp"

namespace station_sim {

    Model::~Model() = default;

    Model::Model(int unique_id, ModelParameters model_parameters) {
        this->model_parameters = model_parameters;
        initialize_model(unique_id);
        print_per_steps = 100;

        history_state = std::vector<std::vector<Point2D>>(
            model_parameters.get_step_limit(), std::vector<Point2D>(model_parameters.get_population_total()));
    }

    Model::Model(const Model &model) {
        std::random_device rd;
        std::array<int, std::mt19937::state_size> seed_data;
        std::generate_n(seed_data.data(), seed_data.size(), std::ref(rd));
        std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
        random_number_generator = std::make_shared<std::mt19937>(std::mt19937(seq));

        model_id = model.model_id;
        status = model.status;
        speed_step = model.speed_step;

        history_collisions_number = model.history_collisions_number;
        history_collision_locations = model.history_collision_locations;
        wiggle_collisions_number = model.wiggle_collisions_number;
        history_wiggle_locations = model.history_wiggle_locations;

        model_parameters = model.model_parameters;

        step_id = model.step_id;
        pop_active = model.pop_active;
        pop_finished = model.pop_finished;
        boundaries = model.boundaries;
        gates_in_locations = model.gates_in_locations;
        gates_out_locations = model.gates_out_locations;
        agents = model.agents;
        std::for_each(agents.begin(), agents.end(),
                      [&](Agent &agent) { agent.set_random_number_generator(random_number_generator); });

        steps_expected = model.steps_expected;
        steps_taken = model.steps_taken;
        steps_delay = model.steps_delay;

        print_per_steps = model.print_per_steps;
        history_state = model.history_state;
    }

    void Model::initialize_model(int unique_id) {
        std::random_device rd;
        std::array<int, std::mt19937::state_size> seed_data;
        std::generate_n(seed_data.data(), seed_data.size(), std::ref(rd));
        std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
        random_number_generator = std::make_shared<std::mt19937>(std::mt19937(seq));

        model_id = unique_id;
        status = ModelStatus::active;
        step_id = 0;
        pop_active = 0;
        pop_finished = 0;
        history_collisions_number = 0;
        wiggle_collisions_number = 0;

        speed_step =
            (model_parameters.get_speed_mean() - model_parameters.get_speed_min()) / model_parameters.get_speed_steps();

        set_boundaries();
        set_gates_locations();

        generate_agents();
    }

    void Model::set_boundaries() {
        boundaries[0].x = 0;                                   // x1
        boundaries[0].y = 0;                                   // y1
        boundaries[1].x = model_parameters.get_space_width();  // x2
        boundaries[1].y = model_parameters.get_space_height(); // y2
    }

    void Model::set_gates_locations() {
        gates_in_locations.resize(model_parameters.get_gates_in());
        gates_out_locations.resize(model_parameters.get_gates_out());

        create_gates(gates_in_locations, 0, model_parameters.get_space_height(), model_parameters.get_gates_in() + 2);
        create_gates(gates_out_locations, model_parameters.get_space_width(), model_parameters.get_space_height(),
                     model_parameters.get_gates_out() + 2);
    }

    void Model::create_gates(std::vector<Point2D> &gates, float x, float y, int gates_number) {
        std::vector<float> result = HelpFunctions::linear_spaced_vector(0, y, gates_number);
        result.erase(result.begin());
        result.pop_back();

        for (unsigned long i = 0; i < result.size(); i++) {
            gates[i].x = x;
            gates[i].y = result[i];
        }
    }

    void Model::generate_agents() {
        for (int i = 0; i < model_parameters.get_population_total(); i++) {
            agents.emplace_back(Agent(i, *this, model_parameters, random_number_generator));
        }
    }

    const std::vector<Point2D> &Model::get_gates_in_locations() const { return gates_in_locations; }

    const std::vector<Point2D> &Model::get_gates_out_locations() const { return gates_out_locations; }

    void Model::step() {
        if (pop_finished < model_parameters.get_population_total() && step_id < model_parameters.get_step_limit() &&
            status == ModelStatus::active) {
            if (model_parameters.is_do_print() && step_id % print_per_steps == 0) {
                std::cout << "\tIteration: " << step_id << "/" << model_parameters.get_step_limit() << std::endl;
            }

            // get agents and move them
            move_agents();

            if (model_parameters.is_do_history()) {
                history_state[step_id] = get_agents_location();
            }

            step_id += 1;
        } else {
            if (pop_finished < model_parameters.get_population_total()) {
                status = ModelStatus::finished;

                if (model_parameters.is_do_print() && status == ModelStatus::active) {
                    std::cout << "StationSim " << model_id << " - Everyone made it!" << std::endl;
                }
            }
        }
    }

    int Model::get_history_collisions_number() const { return history_collisions_number; }

    void Model::set_history_collisions_number(int history_collisions_number) {
        this->history_collisions_number = history_collisions_number;
    }

    void Model::increase_history_collisions_number_by_value(int value_increase) {
        history_collisions_number += value_increase;
    }

    void Model::add_to_history_collision_locations(Point2D new_location) {
        history_collisions_number = +1;
        history_collision_locations.push_back(new_location);
    }

    void Model::increase_wiggle_collisions_number_by_value(int value_increase) {
        wiggle_collisions_number += value_increase;
    }

    void Model::add_to_history_wiggle_locations(Point2D new_location) {
        history_wiggle_locations.push_back(new_location);
    }

    void Model::move_agents() {
        for (auto &agent : agents) {
            agent.step(*this, model_parameters);
        }
    }

    int Model::get_unique_id() const { return model_id; }

    float Model::get_speed_step() const { return speed_step; }

    std::shared_ptr<std::mt19937> Model::get_generator() const { return random_number_generator; }

    std::vector<Point2D> Model::get_agents_location() {
        std::vector<Point2D> agents_locations;

        for (const Agent &agent : agents) {
            agents_locations.push_back(agent.get_agent_location());
        }

        return agents_locations;
    }

    void Model::calculate_print_model_run_analytics() {
        std::cout << "Finish step number: " << step_id << std::endl;
        std::cout << "Population size reached target: " << pop_finished << std::endl;
        std::cout << "Population size active: " << pop_active << std::endl;
        std::cout << "Mean steps taken per agent: "
                  << std::reduce(steps_taken.begin(), steps_taken.end(), 0.0) / agents.size() << std::endl;
        std::cout << "Expected steps per agent: "
                  << std::reduce(steps_expected.begin(), steps_expected.end(), 0.0) / agents.size() << std::endl;
        std::cout << "Mean time delay per agent: "
                  << std::reduce(steps_delay.begin(), steps_delay.end(), 0.0) / agents.size() << std::endl;
        std::cout << "Mean number of collisions per agent: "
                  << std::accumulate(
                         agents.begin(), agents.end(), 0.0,
                         [&](int total, const Agent &agent) { return total += agent.get_history_collisions(); }) /
                         agents.size()
                  << std::endl;
        std::cout << "Mean number of wiggles per agent: "
                  << std::accumulate(
                         agents.begin(), agents.end(), 0.0,
                         [&](int total, const Agent &agent) { return total += agent.get_history_wiggles(); }) /
                         agents.size()
                  << std::endl;
    }

    ModelParameters Model::get_model_parameters() const { return model_parameters; }

    bool Model::model_simulation_finished() { return pop_finished == model_parameters.get_population_total(); }

    void Model::write_model_output_to_hdf_5(std::string file_name) {
        // Create a file
        H5::H5File file(file_name.c_str(), H5F_ACC_TRUNC);
        H5::Group history_group(file.createGroup("/history"));

        // write_model_parameters_to_hdf5(file);
        write_agent_locations_to_hdf_5(history_group);
        write_collisions_history_to_hdf_5(history_group);
        write_wiggle_history_to_hdf_5(history_group);
    }

    // todo
    void Model::write_model_parameters_to_hdf5(H5::Group &model_parameters_group) {}

    void Model::write_agent_locations_to_hdf_5(H5::Group &history_group) {
        H5::Group agents_locations_group(history_group.createGroup("/history/agents_locations"));

        for (Agent &agent : agents) {
            // Create the data space for the dataset.
            hsize_t dims[2];
            dims[0] = agent.get_history_locations().size();
            dims[1] = 2;
            int rank = 2;
            H5::DataSpace dataspace(rank, dims);

            std::string dataset_name("agent_");
            dataset_name.append(std::to_string(agent.get_agent_id()));
            H5::DataSet dataset =
                agents_locations_group.createDataSet(dataset_name.c_str(), H5::PredType::NATIVE_FLOAT, dataspace);

            float agents_locations[dims[0]][2];
            for (int i = 0; i < dims[0]; i++) {
                agents_locations[i][0] = agent.get_history_locations()[i].x;
                agents_locations[i][1] = agent.get_history_locations()[i].y;
            }
            dataset.write(agents_locations, H5::PredType::NATIVE_FLOAT);
        }
    }

    void Model::write_collisions_history_to_hdf_5(H5::Group &history_group) {
        int rank = 2;
        hsize_t dims[2];
        dims[0] = history_collision_locations.size();
        dims[1] = 2;
        H5::DataSpace dataspace(rank, dims);

        H5::DataSet dataset =
            history_group.createDataSet("collisions_locations", H5::PredType::NATIVE_FLOAT, dataspace);

        int temp_size = history_collision_locations.size();
        std::vector<std::vector<float>> collision_locations(temp_size, std::vector<float>(2));
        for (int i = 0; i < history_collision_locations.size(); i++) {

            collision_locations[i][0] = history_collision_locations[i].x;
            collision_locations[i][1] = history_collision_locations[i].y;
        }
        dataset.write(collision_locations.data(), H5::PredType::NATIVE_FLOAT);

        H5::DataSpace attr_dataspace_description(H5S_SCALAR);
        H5::StrType strdatatype(H5::PredType::C_S1, 256);
        const H5std_string strwritebuf("Agents' collision locations");
        H5::Attribute myatt_in = dataset.createAttribute("Description", strdatatype, attr_dataspace_description);
        myatt_in.write(strdatatype, strwritebuf);
    }

    void Model::write_wiggle_history_to_hdf_5(H5::Group &history_group) {
        int rank = 2;
        hsize_t dims[2];
        dims[0] = history_wiggle_locations.size();
        dims[1] = 2;
        H5::DataSpace dataspace(rank, dims);

        H5::DataSet dataset = history_group.createDataSet("wiggle_locations", H5::PredType::NATIVE_FLOAT, dataspace);

        float collision_locations[history_wiggle_locations.size()][2];
        for (int i = 0; i < history_wiggle_locations.size(); i++) {

            collision_locations[i][0] = history_wiggle_locations[i].x;
            collision_locations[i][1] = history_wiggle_locations[i].y;
        }
        dataset.write(collision_locations, H5::PredType::NATIVE_FLOAT);

        H5::DataSpace attr_dataspace_description(H5S_SCALAR);
        H5::StrType strdatatype(H5::PredType::C_S1, 256);
        const H5std_string strwritebuf("Agents' wiggle locations");
        H5::Attribute myatt_in = dataset.createAttribute("Description", strdatatype, attr_dataspace_description);
        myatt_in.write(strdatatype, strwritebuf);
    }

    ModelStatus Model::get_status() const { return status; }

    void Model::reseed_random_number_generator() {
        std::random_device r;
        random_number_generator = std::make_shared<std::mt19937>(std::mt19937(r()));
    }

    const ModelState Model::get_state() const {
        ModelState model_state;

        for (const Agent &agent : agents) {
            model_state.agents_location.push_back(agent.get_agent_location());
            model_state.agent_active_status.push_back(agent.getStatus());
            model_state.agents_desired_location.push_back(agent.get_desired_location());
            }

        return model_state;
    }

    void Model::set_state(const ModelState &new_state) {
        for (unsigned long i = 0; i < new_state.agents_location.size(); i++) {
            agents.at(i).set_agent_location(new_state.agents_location.at(i));
            agents.at(i).set_desired_location(new_state.agents_desired_location.at(i));
        }
    }

    std::vector<float> Model::get_active_agents_state() const {
        std::vector<float> state;
        for (const Agent &agent : agents) {
            if (agent.getStatus() == AgentStatus::active) {
                state.push_back(agent.get_agent_location().x);
                state.push_back(agent.get_agent_location().y);
            }
        }
        return state;
    }

    bool Model::is_active() const { return get_status() == ModelStatus::active; }

} // namespace station_sim