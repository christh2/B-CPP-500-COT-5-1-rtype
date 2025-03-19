#pragma once
#include <iostream>

namespace NovaEngine {
        /**
         * @brief A rectangle box component that defines the entity's physical bounds.
         *
         * This component represents the rectangular bounding box of the entity, which is used for collision detection,
         * spatial interactions, and defining the entity's physical size within the game world.
        */

    typedef struct box_collider {
        double width;
        double height;
        box_collider(double width, double height) : width(width), height(height) {}
    } box_collider_t;
}
