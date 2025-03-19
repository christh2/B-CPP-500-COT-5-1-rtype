#include "../../include/network/server/serveur.hpp"
#include "../../../nova_engine/include/core/system.hpp"
#include "../../../nova_engine/include/core/register.hpp"

//spawn_(le nom de l'énnemie)
std::size_t Enemy::spawn_battle_ship(int x, int y, NovaEngine::Registry &network_register) {
    std::size_t id = network_register.spawn_entity();
    //std::cout << "Battle Ship [" << static_cast<int>(id) << "] enemy created" <<  std::endl;
    network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(x, y));
    network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(0, 0, 0, 0));
    network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
    network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(32, 32));
    network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
    network_register.add_component<NovaEngine::actor_t>(id, NovaEngine::actor_t(1, "Battle Ship"));
    network_register.add_component<NovaEngine::behavior_t>(id, NovaEngine::behavior_t("../assets/scripts/battle.lua", 0.016));
    network_register.add_component<NovaEngine::score_t>(id, NovaEngine::score_t(2));
    network_register.add_entity(id);
    return id;
}

std::size_t Enemy::spawn_pique(int x, int y, NovaEngine::Registry &network_register) {
    std::size_t id = network_register.spawn_entity();
    //std::cout << "Pique enemy [" << static_cast<int>(id) << "] created" <<  std::endl;
    network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(x, y));
    network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(0, 0, 0, 0));
    network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
    network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(112.2, 66.3));
    network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
    network_register.add_component<NovaEngine::actor_t>(id, NovaEngine::actor_t(1, "Pique"));
    network_register.add_component<NovaEngine::score_t>(id, NovaEngine::score_t(1));
    network_register.add_component<NovaEngine::behavior_t>(id, NovaEngine::behavior_t("../assets/scripts/boul.lua", 0.016));
    network_register.add_entity(id);
    return id;
}

std::size_t Enemy::spawn_commandant(int x, int y, NovaEngine::Registry &network_register) {
    std::size_t id = network_register.spawn_entity();
    //std::cout << "Commandant [" << static_cast<int>(id) << "] enemy created" <<  std::endl;
    network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(x, y));
    network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(1.5, 0, -1, 0));
    network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
    network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(50, 55));
    network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
    network_register.add_component<NovaEngine::actor_t>(id, NovaEngine::actor_t(1, "Commandant"));
    network_register.add_component<NovaEngine::score_t>(id, NovaEngine::score_t(8));
    network_register.add_entity(id);
    return id;
}

std::size_t Enemy::spawn_alien(int x, int y, NovaEngine::Registry &network_register) {
    std::size_t id = network_register.spawn_entity();
    //std::cout << "First boss [" << static_cast<int>(id) << "] enemy created" <<  std::endl;
    network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(x, y));
    network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(1.5, 0, -1, 0));
    network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
    network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(260, 144));
    network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
    network_register.add_component<NovaEngine::actor_t>(id, NovaEngine::actor_t(1, "Alien"));
    network_register.add_component<NovaEngine::behavior_t>(id, NovaEngine::behavior_t("../assets/scripts/alien.lua", 0.016, 1500));
    network_register.add_component<NovaEngine::linked_t>(id, NovaEngine::linked_t({0, 1}, false));
   // if (network_register.get_component<NovaEngine::actor_t>(0).has_value()) {
        std::cout << network_register.get_component<NovaEngine::actor_t>(0).value().name << std::endl;
    //}
    network_register.add_component<NovaEngine::score_t>(id, NovaEngine::score_t(10));
    network_register.add_entity(id);
    return id;
}

std::size_t Enemy::spawn_papillon(int x, int y, NovaEngine::Registry &network_register) {
    std::size_t id = network_register.spawn_entity();
    //std::cout << "First boss [" << static_cast<int>(id) << "] enemy created" <<  std::endl;
    network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(x, y));
    network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(0, 0, 0, 0));
    network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
    network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(260, 144));
    network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
    network_register.add_component<NovaEngine::actor_t>(id, NovaEngine::actor_t(1, "Papillon"));
    network_register.add_component<NovaEngine::behavior_t>(id, NovaEngine::behavior_t("../assets/scripts/boul.lua", 0.064));
    network_register.add_component<NovaEngine::score_t>(id, NovaEngine::score_t(40));
    network_register.add_entity(id);
    return id;
}

std::size_t Enemy::spawn_papillon_body(int x, int y, NovaEngine::Registry &network_register) {
    std::size_t id = network_register.spawn_entity();
    //std::cout << "First boss [" << static_cast<int>(id) << "] enemy created" <<  std::endl;
    network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(x, y));
    network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(0, 0, 0, 0));
    network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
    network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(260, 144));
    network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
    network_register.add_component<NovaEngine::actor_t>(id, NovaEngine::actor_t(1, "Papillon"));
    network_register.add_component<NovaEngine::behavior_t>(id, NovaEngine::behavior_t("../assets/scripts/follow.lua", 0.064));
    network_register.add_component<NovaEngine::score_t>(id, NovaEngine::score_t(40));
    network_register.add_entity(id);
    return id;
}

std::size_t Enemy::spawn_papillon_static(int x, int y, NovaEngine::Registry &network_register) {
    std::size_t id = network_register.spawn_entity();
    //std::cout << "First boss [" << static_cast<int>(id) << "] enemy created" <<  std::endl;
    network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(x, y));
    network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(0, 0, 0, 0));
    network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
    network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(260, 144));
    network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
    network_register.add_component<NovaEngine::behavior_t>(id, NovaEngine::behavior_t("../assets/scripts/shield.lua", 0.064));
    network_register.add_component<NovaEngine::actor_t>(id, NovaEngine::actor_t(1, "Papillon"));
    network_register.add_component<NovaEngine::score_t>(id, NovaEngine::score_t(40));
    network_register.add_entity(id);
    return id;
}

std::size_t Enemy::spawn_robot(int x, int y, NovaEngine::Registry &network_register) {
    std::size_t id = network_register.spawn_entity();
    //std::cout << "First boss [" << static_cast<int>(id) << "] enemy created" <<  std::endl;
    network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(x, y));
    network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(0, 0, 0, 0));
    network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
    network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(260, 144));
    network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
    network_register.add_component<NovaEngine::actor_t>(id, NovaEngine::actor_t(1, "Robot"));
    network_register.add_component<NovaEngine::behavior_t>(id, NovaEngine::behavior_t("../assets/scripts/robot_up.lua", 0.016));
    network_register.add_component<NovaEngine::score_t>(id, NovaEngine::score_t(40));
    network_register.add_entity(id);
    return id;
}

std::size_t Enemy::spawn_robot_2(int x, int y, NovaEngine::Registry &network_register) {
    std::size_t id = network_register.spawn_entity();
    //std::cout << "First boss [" << static_cast<int>(id) << "] enemy created" <<  std::endl;
    network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(x, y));
    network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(0, 0, 0, 0));
    network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
    network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(390, 216));
    network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
    network_register.add_component<NovaEngine::actor_t>(id, NovaEngine::actor_t(1, "Robot"));
    network_register.add_component<NovaEngine::behavior_t>(id, NovaEngine::behavior_t("../assets/scripts/robot_down.lua", 0.016));
    network_register.add_component<NovaEngine::score_t>(id, NovaEngine::score_t(40));
    network_register.add_entity(id);
    return id;
}


std::size_t Enemy::spawn_first_boss(int x, int y, NovaEngine::Registry &network_register) {
    std::size_t id = network_register.spawn_entity();
    //std::cout << "Crevette [" << static_cast<int>(id) << "] enemy created" <<  std::endl;
    network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(x, y));
    network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(0, 0, -1, 0));
    network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
    network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(540, 636));
    network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
    network_register.add_component<NovaEngine::actor_t>(id, NovaEngine::actor_t(500, "Crevette"));
    network_register.add_component<NovaEngine::behavior_t>(id, NovaEngine::behavior_t("../assets/scripts/crevette.lua", 0.064));
    network_register.add_component<NovaEngine::linked_t>(id, NovaEngine::linked_t({0, 1}));
    network_register.add_component<NovaEngine::score_t>(id, NovaEngine::score_t(4));
    network_register.add_entity(id);
    return id;
}

std::size_t Enemy::spawn_second_boss(int x, int y, NovaEngine::Registry &network_register) {
    std::size_t id = network_register.spawn_entity();
    //std::cout << "First boss [" << static_cast<int>(id) << "] enemy created" <<  std::endl;
    network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(x, y));
    network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(1.5, 0, -1, 0));
    network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
    network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(198, 240));
    network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
    network_register.add_component<NovaEngine::actor_t>(id, NovaEngine::actor_t(550, "Second boss"));
    network_register.add_component<NovaEngine::behavior_t>(id, NovaEngine::behavior_t("../assets/scripts/gremlin.lua", 0.064));
    network_register.add_component<NovaEngine::score_t>(id, NovaEngine::score_t(40));
    network_register.add_entity(id);
    return id;
}

std::size_t Enemy::spawn_third_boss(int x, int y, NovaEngine::Registry &network_register) {
    std::size_t id = network_register.spawn_entity();
    //std::cout << "First boss [" << static_cast<int>(id) << "] enemy created" <<  std::endl;
    network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(x, y));
    network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(1.5, 0, -1, 0));
    network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
    network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(520, 278));
    network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
    network_register.add_component<NovaEngine::actor_t>(id, NovaEngine::actor_t(660, "Third boss"));
    network_register.add_component<NovaEngine::behavior_t>(id, NovaEngine::behavior_t("../assets/scripts/worm.lua", 0.064));
    network_register.add_component<NovaEngine::score_t>(id, NovaEngine::score_t(40));
    network_register.add_entity(id);
    return id;
}

std::size_t Enemy::spawn_fourth_boss(int x, int y, NovaEngine::Registry &network_register) {
    std::size_t id = network_register.spawn_entity();
    //std::cout << "First boss [" << static_cast<int>(id) << "] enemy created" <<  std::endl;
    network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(x, y));
    network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(1.5, 0, -1, 0));
    network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
    network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(520, 200));
    network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
    network_register.add_component<NovaEngine::actor_t>(id, NovaEngine::actor_t(1000, "Fourth boss"));
    network_register.add_component<NovaEngine::behavior_t>(id, NovaEngine::behavior_t("../assets/scripts/crevette.lua", 0.064));
    network_register.add_component<NovaEngine::score_t>(id, NovaEngine::score_t(40));
    network_register.add_entity(id);
    return id;
}
