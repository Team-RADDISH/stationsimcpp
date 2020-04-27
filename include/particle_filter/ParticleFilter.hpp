//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_PARTICLEFILTER_HPP
#define STATIONSIM_PARTICLEFILTER_HPP

#include "ParticleFilterParameters.hpp"
#include "model/Model.hpp"
#include "model/MultipleModelsRun.hpp"
#include "stationsim_export.h"

namespace station_sim {
    class STATIONSIM_EXPORT ParticleFilter {
      private:
        int number_of_particles;
        int number_of_runs;
        int resample_window;
        bool multi_step;
        float particle_std;
        float target_model_std;
        int agents_to_visualise;
        bool do_save;
        bool do_resample;
        bool plot_save;
        bool do_ani;
        bool show_ani;

        int steps_run;
        int total_number_of_particle_steps_to_run;
        int window_counter;

        std::vector<float> weights;
        std::vector<std::vector<Point2D>> locations;

        std::mt19937 *generator;
        std::normal_distribution<float> float_normal_distribution;

        Model base_model;
        std::vector<Model> models;
        MultipleModelsRun multiple_models_run;

      public:
        ParticleFilter() = delete;

        explicit ParticleFilter(Model base_model);

        ~ParticleFilter();

        /// \brief Step Particle Filter
        ///
        /// Loop through process. Predict the base model and particles
        /// forward. If the resample window has been reached, reweight particles
        /// based on distance to base model and resample particles choosing
        /// particles with higher weights. Then save and animate the data. When
        /// done, plot save figures. Note: if the multi_step is True then
        /// predict() is called once, but steps the model forward until the next
        /// window. This is quicker but means that animations and saves will
        /// only report once per window, rather than every iteration
        void step();

        /// \brief Step the base model
        ///
        /// Increment time. Step the base model. Use a multiprocessing method to
        /// step particle models, set the particle states as the agent
        /// locations with some added noise, and reassign the
        /// locations of the particle agents using the new particle
        /// states. We extract the models and states from the stepped
        /// particles variable.
        /// \param number_of_steps The number of iterations to step (usually either 1, or the  resample window)
        void predict(int number_of_steps = 1);

        /// \brief Step a particle
        ///
        /// Step a particle, assign the locations of the
        /// agents to the particle state with some noise, and
        /// then use the new particle state to set the location
        /// of the agents.
        ///
        /// \param model Μodel object associated with the particle that needs to be stepped
        /// \param num_iter The number of iterations to step
        /// \param particle_std Τhe particle noise standard deviation
        void step_particle(Model &model, int num_iter, float particle_std);

        void reweight();

        float calculate_particle_fit(const Model &model, const std::vector<Point2D> &measured_state);

        void resample();

        void update_agents_locations_of_model(std::vector<Point2D> locations, Model &model);

        void calculate_statistics();
    };
} // namespace station_sim

#endif // STATIONSIM_PARTICLEFILTER_HPP
