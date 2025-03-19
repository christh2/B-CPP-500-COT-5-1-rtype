#pragma once
#include <iostream>
#include <chrono>

namespace NovaEngine {
        /**
         * @brief A gravity component that applies gravitational force to the entity.
         *
         * This component continuously pulls the entity towards the ground or another direction
         * depending on the gravitational settings of the game world, simulating realistic gravity effects.
        */

    typedef struct gravity_t {
        double g;
        std::chrono::time_point<std::chrono::steady_clock> time;
        double speed;
        gravity_t(double g = 0.981) : g(g), speed(0) {
            time = std::chrono::steady_clock::now();
        }
        void clock_restart() {
            time = std::chrono::steady_clock::now();
        }
        float dt() {
            return std::chrono::duration<float>(std::chrono::steady_clock::now() - time).count();
        }
    } gravity_t;
}