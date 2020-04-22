//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include "particle_filter/ParticleFilter.hpp"
#include "HelpFunctions.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

namespace station_sim {

    ParticleFilter::ParticleFilter(Model base_model) {
        this->number_of_particles =
            100; // todo change that param_list[int(sys.argv[1]) - 1][0],  # particles read from ARC task array variable
        this->number_of_runs = 20;
        this->resample_window = 10;
        this->multi_step = true;
        this->particle_std =
            0.5; // todo change that param_list[int(sys.argv[1]) - 1][2], # Particle noise read from task array variable
        this->target_model_std = 1.0;
        this->agents_to_visualise = 10;
        this->do_save = true;
        this->plot_save = false;
        this->do_ani = false;
        this->show_ani = false;
        this->do_resample = true;

        steps_run = 0;
        number_of_step_to_run = 100;
        window_counter = 0;

        std::random_device r;
        generator = new std::mt19937(r());

        this->base_model = base_model;

        locations.resize(number_of_particles);
        std::for_each(locations.begin(), locations.end(),
                      [&base_model](std::vector<Point2D> &point_2d) { point_2d = base_model.get_agents_location(); });

        weights = std::vector<float>(base_model.get_model_parameters()->get_population_total());
        std::fill(weights.begin(), weights.end(), 1.0);

        for (int i = 0; i < base_model.get_model_parameters()->get_population_total(); i++) {
            std::shared_ptr<station_sim::ModelParameters> model_parameters(new station_sim::ModelParameters);
            model_parameters->set_population_total(100);
            model_parameters->set_do_print(false);

            station_sim::Model model(i, model_parameters);

            multiple_models_run.add_model_and_model_parameters(model);
            models.push_back(model);
        }
    }

    ParticleFilter::~ParticleFilter() { delete generator; }

    void ParticleFilter::step() {
        while (steps_run < number_of_step_to_run) {
            std::cout << "Step number: " << steps_run << std::endl;
            int number_of_steps = 1;
            if (multi_step) {
                steps_run += resample_window;
                number_of_steps = resample_window;
            } else {
                steps_run++;
            }

            if (std::any_of(models.cbegin(), models.cend(),
                            [](Model model) { return model.get_status() == ModelStatus::active; })) {

                predict(number_of_steps);

                if (steps_run % resample_window == 0) {
                    window_counter++;

                    if (do_resample) {
                        reweight();
                        resample();
                    }
                }
            }
        }
    }

    void ParticleFilter::predict(int number_of_steps) {
        for (int i = 0; i < number_of_steps; i++) {
            base_model.step();
        }

        for (auto &model : models) {
            step_particle(model, number_of_steps * number_of_particles, particle_std * number_of_particles);
        }
    }

    void ParticleFilter::step_particle(Model &model, int num_iter, float particle_std) {
        model.reseed_random_number_generator();
        for (int i = 0; i < num_iter; i++) {
            model.step();
        }

        float_normal_distribution = std::normal_distribution<float>(0.0, particle_std);

        for (Agent &agent : model.agents) {
            Point2D agent_location = agent.get_agent_location();
            agent_location.x += float_normal_distribution(*generator);
            agent_location.y += float_normal_distribution(*generator);
            agent.set_agent_location(agent_location);
        }
    }

    void ParticleFilter::reweight() {

        // Add noise to the synthetic target data
        float_normal_distribution = std::normal_distribution<float>(0.0, powf(target_model_std, 2));
        std::vector<Point2D> measured_state(base_model.agents.size());
        for (unsigned long i = 0; i < base_model.agents.size(); i++) {
            Point2D agent_location = base_model.agents[i].get_agent_location();
            measured_state[i].x = agent_location.x + float_normal_distribution(*generator);
            measured_state[i].y = agent_location.y + float_normal_distribution(*generator);
        }

        std::vector<double> distance(measured_state.size());
        for (int i = 0; i < models.size(); i++) {
            distance[i] = calculate_particle_fit(models[i], measured_state);
        }

        std::transform(distance.begin(), distance.end(), weights.begin(), [](float distance) -> float {
            return static_cast<float>(1.0 / (pow((static_cast<double>(distance) + 1e-9), 2)));
        });

        double sum = std::reduce(weights.begin(), weights.end(), 0.0);
        std::for_each(weights.begin(), weights.end(),
                      [sum](float &weight) { weight = static_cast<float>(static_cast<double>(weight) / sum); });
    }

    float ParticleFilter::calculate_particle_fit(const Model &model, const std::vector<Point2D> &measured_state) {
        float distance = 0;

        for (int i = 0; i < model.agents.size(); i++) {
            distance += std::sqrt(powf(model.agents[i].get_agent_location().x - measured_state[i].x, 2) +
                                  powf(model.agents[i].get_agent_location().y - measured_state[i].y, 2));
        }

        return distance;
    }

    void ParticleFilter::resample() {
        std::vector<float> offset_partition =
            HelpFunctions::evenly_spaced_values_within_interval(0, number_of_particles, 1);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(0.0, 1.0);
        std::for_each(offset_partition.begin(), offset_partition.end(),
                      [&](float &value) { value = (value + dis(gen)) / number_of_particles; });

        std::vector<float> cumsum(weights.size());
        cumsum[0] = weights[0];
        for (size_t i = 1; i < weights.size(); i++) {
            cumsum[i] = cumsum[i - 1] + weights[i];
        }

        std::vector<int> indexes(number_of_particles);

        {
            int i = 0;
            int j = 0;
            while (i < number_of_particles) {
                if (offset_partition[i] < cumsum[j]) {
                    indexes[i] = j;
                    i++;
                } else {
                    j++;
                }
            }
        }

        std::vector<float> weights_temp(weights);
        for (int i = 0; i < indexes.size(); i++) {
            weights_temp[i] = weights[i];
        }
        for (int i = 0; i < indexes.size(); i++) {
            weights[i] = weights_temp[indexes[i]];
        }

        std::vector<std::vector<Point2D>> locations_temp(locations);
        for (int i = 0; i < indexes.size(); i++) {
            locations_temp[i] = locations[i];
        }
        for (int i = 0; i < indexes.size(); i++) {
            locations[i] = locations_temp[indexes[i]];
        }

        for (int i = 0; i < indexes.size(); i++) {
            update_agents_locations_of_model(locations[i], models[i]);
        }
    }

    void ParticleFilter::update_agents_locations_of_model(std::vector<Point2D> locations, Model &model) {
        for (int i = 0; i < model.agents.size(); i++) {
            model.agents[i].set_agent_location(locations[i]);
        }
    }

    void ParticleFilter::calculate_statistics() {
        // calculate number of active agents in base model
        int base_model_active_agents = 0;
        for (const Agent &agent : base_model.agents) {
            if (agent.getStatus() == AgentStatus::active) {
                base_model_active_agents++;
            }
        }

        std::cout << "Base model active agents: " << base_model_active_agents << std::endl;
    }
} // namespace station_sim
