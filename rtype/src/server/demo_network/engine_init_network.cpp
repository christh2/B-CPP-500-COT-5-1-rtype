#include <iostream>
#include "../../../../nova_engine/include/core/system.hpp"
#include "../../../../nova_engine/include/core/entity.hpp"
#include "../../../../nova_engine/include/core/hybrid_array.hpp"
#include "../../../../nova_engine/include/core/register.hpp"
#include "demo_network.hpp"

namespace NovaEngine {
    void init_nova_component(Registry& network_register) {
        network_register.register_component<velocity_t>();
        network_register.register_component<position_t>();
        network_register.register_component<acceleration_t>();
        network_register.register_component<gravity_t>();
        network_register.register_component<box_collider_t>();
        network_register.register_component<rigid_body_t>();
        network_register.register_component<score_t>();
        network_register.register_component<actor_t>();
        network_register.register_component<weapon_t>();
        network_register.register_component<behavior_t>();
        network_register.register_component<linked_t>();
    }

    void init_nova_system(Registry& network_register) {
        network_register.add_standard_system<position_t, velocity_t, gravity_t>(position_system);
        network_register.add_standard_system<position_t, box_collider_t, rigid_body_t, actor_t, weapon_t, score_t>(collide_detection_system);
        network_register.add_standard_system<weapon_t, position_t>(delete_weapon_system);
        network_register.add_standard_system<actor_t, position_t>(delete_enemy_system);
        network_register.add_standard_system<velocity_t, behavior_t, position_t, linked_t, actor_t>(behavior_system);
        behavior_t::lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);
    }
}
