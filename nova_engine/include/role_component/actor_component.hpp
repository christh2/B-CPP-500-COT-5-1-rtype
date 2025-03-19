#pragma once
#include <iostream>

namespace NovaEngine {
    typedef struct actor_t {
        std::string name;
        int live;
        int level;
        actor_t(int live, std::string name) : name(name), live(live) {
            level = 0;
        }
    } actor_t;
    typedef struct weapon_t {
        std::size_t id;
        std::string name;
        int lives;
        int start;
        int distance;
        weapon_t(std::size_t id, std::size_t live, int start, int distance, std::string name = "") : name(name), id(id), lives(live), start(start), distance(distance) {}
    } weapon_t;
}
