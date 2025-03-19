#pragma once
#include <iostream>

namespace NovaEngine {
        /**
         * @brief A velocity component that controls the speed and direction of the entity.
         *
         * This component keeps track of the entity's movement speed in each direction and updates its position
         * accordingly over time, allowing for smooth motion and directional changes.
        */

    typedef struct velocity_t {
        double speed_x;
        double speed_y;
        double dir_x;
        double dir_y;
        double decel_speed_x;
        double decel_speed_y;

        velocity_t(double x = 0, double y = 0, double dir_x = 1, double dir_y = 1, double decel_speed_x = -1, double decel_speed_y = -1) {
            this->speed_x = x;
            this->speed_y = y;
            this->dir_x = dir_x;
            this->dir_y = dir_y;
            this->decel_speed_x = decel_speed_x;
            this->decel_speed_y = decel_speed_y;
        }
        velocity_t& operator+=(const double vlue) {
            this->speed_x += vlue;
            this->speed_y += vlue;
            return *this;
        }
    } velocity_t;
}
