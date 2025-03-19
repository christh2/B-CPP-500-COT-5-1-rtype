#pragma once
#include <iostream>

namespace NovaEngine {
        /**
         * @brief A force component that applies external forces to an entity.
         *
         * This component is responsible for managing all external forces, such as gravity,
         * wind, or player input, that affect the entity's motion and behavior in the game.
        */
    typedef struct force_t {
        double fx;
        double fy;
        double dirx;
        double diry;
        force_t(double fx = 0, double fy = 0, double dirx = 0, double diry = 0) : fx(fx), fy(fy), dirx(dirx), diry(diry) {}
        force_t(const force_t& other) : fx(other.fx), fy(other.fy), dirx(other.dirx), diry(other.diry) {}
    } force_t;
}
