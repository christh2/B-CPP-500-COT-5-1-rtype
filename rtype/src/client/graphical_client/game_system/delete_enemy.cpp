#include <iostream>
#include "../../../../../nova_engine/include/core/system.hpp"
#include "../../../../../nova_engine/include/core/entity.hpp"
#include "../../../../../nova_engine/include/core/hybrid_array.hpp"

namespace NovaEngine {
    void delete_enemy_system(std::vector<Entity> entities, message_t &message_queue, hybrid_array<actor_t>& actors,
    hybrid_array<position_t>& positions) {
        for (auto& entity : entities) {
            auto &actor = actors[entity];
            auto pos = positions[entity];
            if (actor.has_value() && pos.has_value()) {
                if ((pos.value()._x < -500) && actor.value().name != "Player") {
                    actor.value().live = 0;
                }
            }
        }
    }
}
