//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#ifndef PARTICLE_FILTER_PARTICLEFILTERDATAFEED_HPP
#define PARTICLE_FILTER_PARTICLEFILTERDATAFEED_HPP

namespace particle_filter {
    template <class StateType>
    class ParticleFilterDataFeed {
      public:
        ParticleFilterDataFeed() = default;
        virtual ~ParticleFilterDataFeed() = default;

        virtual void progress_feed() = 0;
        [[nodiscard]] virtual StateType get_state() = 0;
        virtual void print_statistics() = 0;
    };
} // namespace particle_filter

#endif // PARTICLE_FILTER_PARTICLEFILTERDATAFEED_HPP
