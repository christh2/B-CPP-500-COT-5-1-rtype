#pragma once
#include <iostream>
#include "../../nova_engine/include/core/system.hpp"
#include "../../nova_engine/include/core/register.hpp"

namespace NovaEngine {
    void init_nova_component(Registry& network_register);
    void init_nova_system(Registry& network_register);
    // void delete_weapon_system(std::vector<Entity> entities, hybrid_array<weapon_t>& weapons,
    //     hybrid_array<position_t>& positions);
}