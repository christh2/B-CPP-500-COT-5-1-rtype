#include <iostream>
#include "../../include/core/system.hpp"
#include "../../include/core/entity.hpp"
#include "../../include/core/hybrid_array.hpp"
#include "../../include/core/system.hpp"

namespace NovaEngine {
    void collision_response_system(std::vector<Entity> entities, message_t &message_queue, hybrid_array<rigid_body_t>& bodies, hybrid_array<score_t>& scores,
    hybrid_array<actor_t>& actors, hybrid_array<weapon_t>& weapons) {
        for (auto& i : entities) {
            auto &body = bodies[i];
            auto &actor = actors[i];
            auto &weapon = weapons[i];
            if (body.has_value() && body.value().is_touched && body.value().id != -1) {
                auto &body_vlue = body.value();
                if (actor.has_value() && actors[body_vlue.id].has_value()) {
                    actor_t &actor_vlue = actor.value();
                    if (actor.value().name == "Player")
                        std::cout << "\e[1;31m " << actor_vlue.name << " Hit another actor " << actors[body_vlue.id].value().name << "\e[0m" << std::endl;
                    else
                      std::cout << "\e[1;31m " << actor_vlue.name << std::endl;
//                    actor_vlue.live -= 1;
                } else if (actor.has_value() && weapons[body_vlue.id].has_value()) {
                    actor_t &actor_vlue = actor.value();
                 //   actor_vlue.live -= 1;
                } else if (weapon.has_value() && actors[body_vlue.id].has_value()) {
                    weapon_t &weapon_vlue = weapon.value();
                    actor_t &actor_vlue = actors[weapon_vlue.id].value();
                    actor_t &other_actor_vlue = actors[body_vlue.id].value();
                    std::cout << "\e[1;31mWeapon of " << actor_vlue.name << " hitting " << other_actor_vlue.name << "\e[0m" << std::endl;
                    if (other_actor_vlue.name != actor_vlue.name) {
                      //  weapon.value().lives -= 1;
                        std::cout << "\e[1;31mWeapon of " << actor_vlue.name << " hitting " << other_actor_vlue.name << "\e[0m" << std::endl;
                        if (scores[weapon_vlue.id].has_value() && scores[body_vlue.id].has_value()) {
                            scores[weapon_vlue.id].value() += scores[body_vlue.id].value();
                        }
                    } else {
                        std::cout << "\e[1;31mWeapon of " << actor_vlue.name << " hitting " << other_actor_vlue.name << "\e[0m" << std::endl;
                    }
                }
                body.value().is_touched = false;
                body.value().id = -1;
            }
        }
    }
}
