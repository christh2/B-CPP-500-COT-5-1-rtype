#include <iostream>
#include "../../../../../nova_engine/include/core/system.hpp"
#include "../../../../../nova_engine/include/core/entity.hpp"
#include "../../../../../nova_engine/include/core/hybrid_array.hpp"

namespace NovaEngine {
    void delete_weapon_system(std::vector<Entity> entities, message_t &message_queue, hybrid_array<weapon_t>& weapons,
    hybrid_array<position_t>& positions) {
        for (auto& entity : entities) {
            auto &weapon = weapons[entity];
            auto pos = positions[entity];
            if (weapon.has_value() && pos.has_value()) {
                if ((pos.value()._x < 0) || (pos.value()._x > 1900)) {
                    weapon.value().lives = 0;
                }
            }
        }
    }
}