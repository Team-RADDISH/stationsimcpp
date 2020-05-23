//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef STATIONSIM_PARTICLEFILTERDATAFEED_HPP
#define STATIONSIM_PARTICLEFILTERDATAFEED_HPP

#include "stationsim_export.h"

namespace station_sim {
    template <class StateType>
    class STATIONSIM_EXPORT ParticleFilterDataFeed {
      public:
        ParticleFilterDataFeed() = default;
        virtual ~ParticleFilterDataFeed() = default;

        virtual void progress_feed() = 0;
        [[nodiscard]] virtual StateType get_state() = 0;
        virtual void print_statistics() = 0;
    };
} // namespace station_sim

#endif // STATIONSIM_PARTICLEFILTERDATAFEED_HPP
