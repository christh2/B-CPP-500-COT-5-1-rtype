#include <iostream>
#include "../../include/core/system.hpp"
#include "../../include/core/entity.hpp"
#include "../../include/core/hybrid_array.hpp"

namespace NovaEngine {
    void controll_system(std::vector<Entity> entities, message_t &message_queue, hybrid_array<velocity_t>& velocities,
    hybrid_array<controllable_t>& controllables, hybrid_array<acceleration_t>& accelerations) {
        for (auto &i : entities) {
            auto &vel = velocities[i];
            auto &control = controllables[i];
            auto &accel = accelerations[i];
            if (control.has_value() && control.value().is_controllable && vel.has_value() && accel.has_value()) {
                velocity_t &vel_vlue = vel.value();
                acceleration_t tmp_acc(0, 0);
                controllable_t &control_vlue = control.value();
                if (accel.has_value()) {
                    tmp_acc = accel.value();
                }
                if (!control_vlue.control.empty()) {
                    if (control_vlue.control == "R") {
                        vel_vlue.speed_x = 17;
                        vel_vlue.dir_x = 1;
                        vel_vlue.speed_y = 0;
                    } else if (control_vlue.control == "L") {
                        vel_vlue.speed_x = 17;
                        vel_vlue.speed_y = 0;
                        vel_vlue.dir_x = -1;
                    } else if (control_vlue.control == "U") {
                        vel_vlue.speed_y = 17;
                        vel_vlue.speed_x = 0;
                        vel_vlue.dir_y = -1;
                    } else if (control_vlue.control == "D") {
                        vel_vlue.speed_y = 17;
                        vel_vlue.speed_x = 0;
                        vel_vlue.dir_y = 1;
                    }
                    control_vlue.control = "";
                } else {
                    vel_vlue.speed_y = 0;
                    vel_vlue.speed_x = 0;
                    vel_vlue.dir_y = 0;
                    vel_vlue.dir_x = 0;
                    // if (tmp_acc.acc_x <= 0)
                    //     vel_vlue.speed_x = 0;
                    // else {
                    //     vel_vlue.speed_x -= tmp_acc.acc_x;
                    //     if (vel_vlue.speed_x < 0) {
                    //         vel_vlue.speed_x = 0;
                    //         tmp_acc.clock_restart();
                    //     }
                    // }
                    // if (tmp_acc.acc_y <= 0)
                    //     vel_vlue.speed_y = 0;
                    // else {
                    //     vel_vlue.speed_y -= tmp_acc.acc_y;
                    //     if (vel_vlue.speed_y < 0) {
                    //         vel_vlue.speed_y = 0;
                    //         tmp_acc.clock_restart();
                    //     }
                    // }
                }
            }
        }
    }
}
