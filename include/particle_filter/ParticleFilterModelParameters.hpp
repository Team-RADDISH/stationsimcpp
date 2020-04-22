//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_PARTICLEFILTERMODELPARAMETERS_HPP
#define STATIONSIM_PARTICLEFILTERMODELPARAMETERS_HPP

#include "stationsim_export.h"

namespace station_sim {
    class STATIONSIM_EXPORT ParticleFilterModelParameters {
      private:
        int width;
        int height;
        int pop_total;
        int speed_min;
        float separation;
        int batch_iterations;
        bool do_history;
        bool do_print;

      public:
        ParticleFilterModelParameters();

        ~ParticleFilterModelParameters();

        [[nodiscard]] int get_width() const;

        void set_width(int width);

        [[nodiscard]] int get_height() const;

        void set_height(int height);

        [[nodiscard]] int get_pop_total() const;

        void set_pop_total(int pop_total);

        [[nodiscard]] int get_speed_min() const;

        void set_speed_min(int speed_min);

        [[nodiscard]] float get_separation() const;

        void set_separation(float separation);

        [[nodiscard]] int get_batch_iterations() const;

        void set_batch_iterations(int batch_iterations);

        [[nodiscard]] bool is_do_history() const;

        void set_do_history(bool do_history);

        [[nodiscard]] bool is_do_print() const;

        void set_do_print(bool do_print);
    };
} // namespace station_sim

#endif // STATIONSIM_PARTICLEFILTERMODELPARAMETERS_HPP
