//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#include "ParticleFilter.hpp"
#include <algorithm>
#include <vector>

namespace station_sim {

    ParticleFilter::ParticleFilter(Model base_model)
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
        this->do_resample = true;

        steps_run = 0;
        number_of_step_to_run = 10;
        window_counter = 0;

        std::random_device r;
        generator = new std::mt19937(r());

        this->base_model = base_model;

        weights = std::vector<float>(base_model.get_model_parameters()->get_population_total());

        for (int i = 0; i<base_model.get_model_parameters()->get_population_total(); i++) {

            std::shared_ptr<station_sim::ModelParameters> model_parameters(new station_sim::ModelParameters);
            model_parameters->set_population_total(100);
            model_parameters->set_do_print(false);

            station_sim::Model model(i, model_parameters);

            multiple_models_run.add_model_and_model_parameters(model);
        }
    }

    ParticleFilter::~ParticleFilter()
    {
        delete generator;
    }

    void ParticleFilter::step()
    {
        while (steps_run<number_of_step_to_run) {
            int numiter = 1;
            if (multi_step) {
                steps_run += resample_window;
                numiter = resample_window;
            }
            else {
                steps_run++;
            }

            if (multiple_models_run.has_any_model_active()) {
                predict(numiter);
            }

            if (steps_run%resample_window==0) {
                window_counter++;

                if (do_resample) {
                    reweight();
                    resample();
                }
            }
        }
    }

    void ParticleFilter::predict(int numiter)
    {
        for (int i = 0; i<numiter; i++) {
            base_model.step();
        }

        for (int i = 0; i<multiple_models_run.get_size_of_models_vector(); i++) {
            step_particle(multiple_models_run.get_model(i), numiter*number_of_particles,
                    particle_std*number_of_particles);
        }


    }

    void
    ParticleFilter::step_particle(Model& model, int num_iter, float particle_std)
    {
        model.reseed_random_number_generator();
        for (int i = 0; i<num_iter; i++) {
            model.step();
        }

        float_normal_distribution = std::normal_distribution<float>(0.0, particle_std);

        for (Agent& agent:model.agents) {
            Point2D agent_location = agent.get_agent_location();
            agent_location.x += float_normal_distribution(*generator);
            agent_location.y += float_normal_distribution(*generator);
            agent.set_agent_location(agent_location);
        }

    }

    void ParticleFilter::reweight()
    {
        float_normal_distribution = std::normal_distribution<float>(0.0, particle_std);

        std::vector<Point2D> measured_state(base_model.agents.size());

        for (unsigned long i = 0; i<base_model.agents.size(); i++) {
            Point2D agent_location = base_model.agents[i].get_agent_location();
            agent_location.x += float_normal_distribution(*generator);
            agent_location.y += float_normal_distribution(*generator);

            measured_state[i].x = base_model.agents[i].get_agent_location().x-agent_location.x;
            measured_state[i].y = base_model.agents[i].get_agent_location().y-agent_location.y;
        }

        std::vector<double> distance;
        // calculate the norm
        for (Point2D& point_2d: measured_state) {
            distance.push_back(sqrt(static_cast<double>(powf(point_2d.x, 2)+powf(point_2d.y, 2))));
        }

        double sum = 0;
        for (unsigned int i = 0; i<weights.size(); i++) {
            weights[i] = 1.0/static_cast<double>(powf((distance[i]+1e-9), 2));
            sum += weights[i];
        }

        for (float& weight : weights) {
            weight = static_cast<float>(weight/sum);
        }
    }

    void ParticleFilter::resample()
    {

    }
}
