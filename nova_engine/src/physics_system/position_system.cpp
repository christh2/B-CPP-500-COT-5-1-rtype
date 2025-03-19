#include <iostream>
#include "../../include/core/register.hpp"
#include "../../include/core/system.hpp"
#include "../../include/core/hybrid_array.hpp"

namespace NovaEngine {
    void position_system(std::vector<Entity> entities, message_t &message_queue, hybrid_array<position_t>& positions, hybrid_array<velocity_t>& velocities, hybrid_array<gravity_t>& gravities) {
        for (auto &i : entities) {
            auto &pos = positions[i];
            auto &vel = velocities[i];
            auto &gravity = gravities[i];
            if (pos.has_value() && vel.has_value()) {
                position_t &pos_vlue = pos.value();
                velocity_t &vel_vlue = vel.value();
                pos_vlue._x += vel_vlue.speed_x * vel_vlue.dir_x;
                pos_vlue._y += vel_vlue.speed_y * vel_vlue.dir_y;
                if (gravity.has_value()) {
                    pos_vlue._y += gravity.value().speed;
                }
            }
        }
    }
}
