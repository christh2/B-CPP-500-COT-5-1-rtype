#pragma once
#include <iostream>

namespace NovaEngine {
        /**
         * @brief A ground component that defines the entity's interaction with the ground.
         *
         * This component handles the logic for detecting when the entity is on the ground,
         * managing ground-based movement, and preventing the entity from falling through surfaces.
        */

    typedef struct ground {
        bool is_ground;
        ground(bool ground = false) : is_ground(ground) {}
    } ground_t;
}
