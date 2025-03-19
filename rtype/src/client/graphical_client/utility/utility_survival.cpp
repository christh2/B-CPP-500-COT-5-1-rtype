#include <iostream>
#include "../../../../../nova_engine/include/core/system.hpp"
#include "../../../../../nova_engine/include/core/entity.hpp"
#include "../../../../../nova_engine/include/core/hybrid_array.hpp"
#include "../../../../../nova_engine/include/core/register.hpp"
#include "../../../../include/core/game.hpp"
#include <map>

void add_survival_player(NovaEngine::Registry& registry, std::size_t id, NovaEngine::position_t pos, std::string skin) {
    int y = 0;
    if (skin == "blue")
        y = 0;
    if (skin == "pink")
        y = 1;
    if (skin == "red")
        y = 2;
    if (skin == "green")
        y = 3;
    registry.add_component<NovaEngine::actor_t>(id, NovaEngine::actor_t(3, "Player"));
    registry.add_component<NovaEngine::position_t>(id, pos);
    registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("hero", 1, 1, 1, 0, y * 15, 33, 20));
    registry.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t());
    registry.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
    registry.add_component<NovaEngine::controllable_t>(id, NovaEngine::controllable_t());
    registry.add_entity(id);
}

void attack(NovaEngine::Registry& registry, std::size_t shooter, std::string weapon, int offx, int offy, NovaEngine::velocity_t vel)
{
    auto &pos_opt = registry.get_component<NovaEngine::position_t>(shooter);
    if (!pos_opt.has_value())
        return;
    auto &pos = pos_opt.value();
    std::cout << "pos: " << pos._x << " " << pos._y << std::endl;
    std::size_t id = registry.spawn_entity();
    registry.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(pos._x + offx, pos._y + offy));
    registry.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(vel));
    registry.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
    registry.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
    registry.add_component<NovaEngine::weapon_t>(id, NovaEngine::weapon_t(shooter, 1, pos._x, 800, "basic weapon"));
    if (weapon == "basic_weapon") {
        registry.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(53, 12));
        registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("weapon", 0, 1, 1, 196, 114, 37, 20));
    }
    registry.add_entity(id);
}

void add_boul_enemy(NovaEngine::Registry &registry)
{
    std::size_t id = registry.spawn_entity();
    int x = 1910;
    int y = rand() % 900;
    //std::cout << "Boul enemy [" << static_cast<int>(id) << "] created" <<  std::endl;
    registry.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(x, y));
    registry.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(0, 0, 0, 0));
    registry.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
    registry.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(17, 17));
    registry.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
    registry.add_component<NovaEngine::actor_t>(id, NovaEngine::actor_t(1, "Boul"));
    registry.add_component<NovaEngine::score_t>(id, NovaEngine::score_t(1));
    registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("Crevette", 0, 1, 200, 0, 0, 180, 210, sf::Vector2f(3, 3)));
    // registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("Boul", 8, 1, 200, 0, 0, 17, 17, sf::Vector2f(3, 3)));
    registry.add_component<NovaEngine::behavior_t>(id, NovaEngine::behavior_t("../assets/scripts/boul_sur.lua", 0.064));
    registry.add_entity(id);
}

void spawn_ennemy(NovaEngine::Registry &registry, std::string name, int level = 2)
{
    int num_enemy;
    if (level <= 1)
        level = 2;
    num_enemy = rand() % (level);
    for (int i = 0; i < num_enemy; i++) {
        if (name == "boul")
            add_boul_enemy(registry);
    }
}

void handle_attack_request(NovaEngine::Registry& registry)
{
    auto &positions = registry.get_registry<NovaEngine::position_t>();
    for (auto &it : registry.get_message_queue()) {
        std::stringstream ss(it);
        std::string sys, order;
        std::getline(ss, sys, ':');
        std::getline(ss, order, ':');
        std::stringstream ss_order(order);
        std::string type, value;
        int shooter, posx, posy, offx, offy;
        ss_order >> type >> value >> offx >> offy >> shooter;
        std::cout << "Type: " << type << " Value: " << value << " Shooter: " << shooter << std::endl;
        if (type == "shoot" && sys == "Shoot system") {
            attack(registry, shooter, value, offx, offy, NovaEngine::velocity_t(0.2, 0, 1, 0));
        } else if (type == "shoot" && sys == "Behavior order") {
            attack(registry, shooter, value, offx, offy, NovaEngine::velocity_t(-0.2, 0, 1, 0));
        }
    }
    auto &actors = registry.get_registry<NovaEngine::actor_t>();
    auto &weapons = registry.get_registry<NovaEngine::weapon_t>();
    for (auto &it : registry.get_entities()) {
        auto &actor = actors[it];
        auto &weapon = weapons[it];
        if (actor.has_value() && actor.value().live <= 0) {
            registry.kill_entity(it);
        }
        if (weapon.has_value() && weapon.value().lives <= 0) {
            registry.kill_entity(it);
        }
    }
    registry.get_message_queue().clear();
    auto &live_opt = registry.get_component<NovaEngine::actor_t>(9);
    auto &score_opt = registry.get_component<NovaEngine::score_t>(9);
    if (live_opt.has_value()) {
        std::size_t lives = live_opt.value().live;
        if (lives == 2)
            registry.kill_entity(6);
        else if (lives == 1)
            registry.kill_entity(5);
        else if (lives == 0)
            registry.kill_entity(4);
    }
    if (score_opt.has_value()) {
        NovaEngine::text_t &score = registry.get_component<NovaEngine::text_t>(3).value();
        score.text = "Score: " + std::to_string(score_opt.value()) + " pts";
    }
}
