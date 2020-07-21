//---------------------------------------------------------------------------//
// Copyright (c) 2020 Eleftherios Avramidis <ea461@cam.ac.uk>
// Research Computing Services, University of Cambridge, UK
//
// Distributed under The MIT License (MIT)
// See accompanying file LICENSE
//---------------------------------------------------------------------------//

#define CATCH_CONFIG_MAIN

#include <iostream>
#include <memory>

#include "Particle.hpp"
#include "ParticleFilter.hpp"
#include "ParticleState.hpp"
#include "catch.hpp"

using namespace particle_filter;

class TestParticleState : public ParticleState {
  public:
    TestParticleState() = default;
    ~TestParticleState() override = default;

    void mpi_send_state(int rank_destination) override{};
    void mpi_receive_state(int rank_source) override{};
};

class TestParticle : public Particle<TestParticleState> {
  public:
    TestParticle() = default;
    ~TestParticle() override = default;

    [[nodiscard]] bool is_active() const override{};
    [[nodiscard]] const TestParticleState get_state() const override{};
    void perturb_state(float standard_deviation) override{};
    void step() override{};
};

TEST_CASE("Test ParticleFilter API") {
    ParticleFilter<TestParticle, TestParticleState> particle_filter;

    SECTION("Test generated locations") {
        //		std::cout << agent.get_agent_location()[0] << std::endl;

        //		std::cout << agent.get_agent_speed() << std::endl;

        REQUIRE(5 == 5);
    }
}