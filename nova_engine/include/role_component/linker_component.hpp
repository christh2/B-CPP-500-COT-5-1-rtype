#pragma once
#include <iostream>
#include <vector>
#include "../physics_component/position_component.hpp"
#include "../physics_component/velocity_component.hpp"

namespace NovaEngine {
    typedef struct linked_t {
        std::vector<std::size_t> links;
        std::map<std::size_t, position_t> friend_pos;
        std::map<std::size_t, velocity_t> friend_vel;
        bool lock;
        linked_t(std::vector<std::size_t> init, bool locked = true) : lock(locked) {
            for (auto &i : init) {
                links.push_back(i);
                friend_pos[i] = position_t();
                friend_vel[i] = velocity_t();
            }
        }

        void remove_entity(std::size_t id) {
            links.erase(std::remove(links.begin(), links.end(), id), links.end());
            friend_pos.erase(id);
            friend_vel.erase(id);
        }
    } linked_t;
}
