#pragma once
#include <iostream>

namespace NovaEngine {
        /**
         * @brief A rigid body component that defines the physics-based properties of the entity.
         *
         * This component allows the entity to interact with the physical environment, applying realistic physics laws such as
         * collision responses, forces, and rotations, ensuring that the entity behaves according to real-world dynamics.
        */

    typedef struct rigid_body {
        bool is_rigid;
        bool is_touched;
        int id = -1;
        rigid_body(bool rigid = true) : is_rigid(rigid), is_touched(false) {}
    } rigid_body_t;
}
