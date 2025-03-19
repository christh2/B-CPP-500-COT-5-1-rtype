#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

namespace NovaEngine {
    typedef struct controllable_t {
        bool is_controllable;
        std::string control;
        controllable_t() : is_controllable(true), control("") {}
    } controllable_t;
}
