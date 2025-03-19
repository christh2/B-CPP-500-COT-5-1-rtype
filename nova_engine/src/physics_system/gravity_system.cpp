#include <iostream>
#include "../../include/core/system.hpp"
#include "../../include/core/entity.hpp"
#include "../../include/core/hybrid_array.hpp"

namespace NovaEngine {
    void gravity_system(std::vector<Entity> entities, message_t &message_queue, hybrid_array<position_t>& positions, hybrid_array<velocity_t>& velocities, hybrid_array<gravity_t>& gravities, hybrid_array<mass_t>& masses, hybrid_array<ground_t>& grounds) {
        for (auto& i : entities) {
            auto &mass = masses[i];
            auto &gravity = gravities[i];
            auto &position = positions[i];
            if (mass.has_value() && gravity.has_value() && position.has_value()) {
                mass_t mass_vlue = mass.value();
                gravity_t& gravity_vlue = gravity.value();
                position_t &pos_vlue = position.value();
                double tmp;
                gravity_vlue.speed += (gravity_vlue.g * mass_vlue.mass) * gravity_vlue.dt();
                gravity_vlue.clock_restart();
                tmp = gravity_vlue.speed * gravity_vlue.dt() + pos_vlue._y;
            }
        }
    }
}