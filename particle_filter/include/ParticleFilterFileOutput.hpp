//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef PARTICLE_FILTER_PARTICLEFILTERFILEOUTPUT_HPP
#define PARTICLE_FILTER_PARTICLEFILTERFILEOUTPUT_HPP

#include "H5Cpp.h"
#include "ParticleFilter.hpp"

#include <memory>
#include <vector>

namespace particle_filter {
    template <class StateType>
    class ParticleFilterFileOutput {
      public:
        ParticleFilterFileOutput() = default;
        ~ParticleFilterFileOutput() = default;

        void write_particle_filter_data_to_hdf_5(std::string file_name,
                                                 const std::vector<StateType> &particles_states) {
            // Create a file
            H5::H5File file(file_name.c_str(), H5F_ACC_TRUNC);
            H5::Group particle_states_group(file.createGroup("/particle_filter"));

            // write_model_parameters_to_hdf5(file);
            write_particles_states_to_hdf_5(particle_states_group, particles_states);
            //            write_collisions_history_to_hdf_5(history_group);
            //            write_wiggle_history_to_hdf_5(history_group);
        }

        void write_particles_states_to_hdf_5(H5::Group &particle_states_group,
                                             const std::vector<StateType> &particles_states) {

            H5::Group agents_locations_group(particle_states_group.createGroup("/particle_filter/particles_states"));

            int particle_count = 0;
            for (const StateType &particles_state : particles_states) {
                // Create the data space for the dataset.
                hsize_t dims[1];
                dims[0] = particles_state.size();
                int rank = 1;
                H5::DataSpace dataspace(rank, dims);

                std::string dataset_name("particle_");
                dataset_name.append(std::to_string(particle_count++));
                H5::DataSet dataset =
                    agents_locations_group.createDataSet(dataset_name.c_str(), H5::PredType::NATIVE_FLOAT, dataspace);

                dataset.write(particles_state.data(), H5::PredType::NATIVE_FLOAT);
            }
        }
    };
} // namespace particle_filter

#endif // PARTICLE_FILTER_PARTICLEFILTERFILEOUTPUT_HPP
