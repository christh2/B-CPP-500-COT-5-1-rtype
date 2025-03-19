#include <iostream>
#include "../../include/core/system.hpp"
#include "../../include/core/system.hpp"
#include "../../include/core/entity.hpp"
#include "../../include/core/hybrid_array.hpp"

namespace NovaEngine {
    void snack_system(std::vector<Entity> entities, message_t &message_queue, hybrid_array<snackbar_t>& snacks) {
        for (auto &i : entities) {
            auto &snack = snacks[i];
            if (snack.has_value()) {
                auto &snack_value = snack.value();
                if (snack_value.is_expired() && !snack_value.dead) {
                    message_queue.push_back("Snack_expires " + std::to_string(i));
                    snack_value.dead = true;
                }
                if (!snack_value.start) {
                    snack_value.restart();
                    snack_value.start = true;
                }
            }
        }
    }
}
