#pragma once
#include <iostream>

namespace NovaEngine {
        /**
         * @brief A position component that tracks the entity's location in the game world.
         *
         * This component stores and updates the entity's coordinates in the game world,
         * ensuring that its position is accurately reflected as it moves or interacts with the environment.
        */

    typedef struct position_t{
        double _x;
        double _y;
        position_t(double x, double y) : _x(x), _y(y) {}
        position_t() : _x(0), _y(0) {}
        position_t(const position_t &other) : _x(other._x), _y(other._y) {}
        void operator=(const position_t &other) {
            _x = other._x;
            _y = other._y;
        }
    } position_t;
}