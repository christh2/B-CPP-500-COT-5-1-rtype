#include <iostream>
#include "../../include/core/system.hpp"
#include "../../include/core/system.hpp"
#include "../../include/core/entity.hpp"
#include "../../include/core/hybrid_array.hpp"

namespace NovaEngine {
    bool opposite_side(std::string camp1, std::string camp2) {
        std::size_t pos = camp1.find("Player");
        std::size_t pos2 = camp2.find("Player");        
        if (camp1 == "Player" && camp2 != "Player")
            return true;
        if (camp1 != "Player" && camp2 == "Player")
            return true;
        return false;
    }

    void collide_response(Entity entity, Entity other_entity, hybrid_array<actor_t>& actors, hybrid_array<weapon_t>& weapons,
     hybrid_array<score_t>& scores, rigid_body_t &r1, rigid_body_t &r2, message_t &message) {
        auto &actor = actors[entity];
        auto &other_actor = actors[other_entity];
        auto &weapon = weapons[entity];
        auto &other_weapon = weapons[other_entity];
        if (actor.has_value() && other_actor.has_value() && opposite_side(actor.value().name, other_actor.value().name)) {
            actor_t &actor_vlue = actor.value();
            r1.is_touched = true;
            r1.id = other_entity;
            r2.is_touched = true;
            r2.id = entity;
            if (actor.value().name == "Player") {
                actor_vlue.live -= 1;
            }
        } else if (weapon.has_value() && weapon.value().name != "bonus" && other_actor.has_value() && actors[weapon.value().id].has_value()) {
            weapon_t &weapon_vlue = weapon.value();
            actor_t &other_actor_vlue = other_actor.value();
            actor_t &actor_vlue = actors[weapon_vlue.id].value();
            if (opposite_side(actor_vlue.name, other_actor_vlue.name) && weapon_vlue.name != "bonus") {
                weapon.value().lives = 0;
                other_actor_vlue.live -= 1;
                r1.is_touched = true;
                r1.id = other_entity;
                r2.is_touched = true;
                r2.id = entity; 
                //std::cout << "\e[1;34m " << actor_vlue.name << " of id " << weapon_vlue.id << " Hit another actor " << other_actor_vlue.name << "\e[0m" << std::endl;
                if (scores[weapon_vlue.id].has_value()) {
                    scores[weapon_vlue.id].value() += 100;
                    //std::cout << "\e[1;32m Scores of " << weapon_vlue.id << " is " << scores[weapon_vlue.id].value() << "\e[0m" << std::endl;
                }
            }
        } else if (other_actor.has_value() && weapon.has_value()) {
            weapon_t &weapon_vlue = weapon.value();
            if (weapon_vlue.name == "bonus" && other_actor.value().name == "Player") {
                weapon_vlue.lives = 0;
                message.push_back("Collide order:get_bonus " + std::to_string(other_entity));
                std::cout << "\033[0;32mCollided\033[0m" << std::endl;
            }
        }
    }

    void collide_detection_system(std::vector<Entity> entities, message_t& message_queue, hybrid_array<position_t>& positions,
    hybrid_array<box_collider_t>& collider, hybrid_array<rigid_body_t>& bodies,
    hybrid_array<actor_t>& actors, hybrid_array<weapon_t>& weapons, hybrid_array<score_t>& scores) {
        for (auto& i : entities) {
            auto &box = collider[i];
            auto &position = positions[i];
            auto &body = bodies[i];
            if (box.has_value() && position.has_value() &&
                body.has_value() && body.value().is_rigid) {
                for (auto &j : entities) {
                    auto &other_box = collider[j];
                    auto &other_position = positions[j];
                    auto &other_body = bodies[j];
                    if (j != i && other_box.has_value() && other_position.has_value() &&
                        other_body.has_value() && other_body.value().is_rigid) {
                        position_t &box_pos = position.value();
                        position_t &other_box_pos = other_position.value();
                        rigid_body_t &body_vlue = other_body.value();
                        rigid_body_t &other_body_vlue = other_body.value();
                        if ((box_pos._x + box.value().width >= other_box_pos._x) &&
                            (box_pos._x <= other_box_pos._x + other_box.value().width) &&
                            (box_pos._y + box.value().height >= other_box_pos._y) &&
                            (box_pos._y <= other_box_pos._y + other_box.value().height)) {
                                collide_response(i, j, actors, weapons, scores, body_vlue, other_body_vlue, message_queue);
                        }
                    }
                }
            }
        }
    }
}
