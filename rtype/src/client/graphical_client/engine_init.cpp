#include <iostream>
#include "../../../../nova_engine/include/core/system.hpp"
#include "../../../../nova_engine/include/core/entity.hpp"
#include "../../../../nova_engine/include/core/hybrid_array.hpp"
#include "../../../../nova_engine/include/core/register.hpp"
#include "../../../include/core/game.hpp"

namespace NovaEngine {
    void init_nova_component(Registry& registry) {
        registry.register_component<controllable_t>();
        registry.register_component<sprite_t>();
        registry.register_component<text_t>();
        registry.register_component<acceleration_t>();
        registry.register_component<velocity_t>();
        registry.register_component<position_t>();
        registry.register_component<rigid_body_t>();
        registry.register_component<box_collider_t>();
        registry.register_component<actor_t>();
        registry.register_component<weapon_t>();
        registry.register_component<score_t>();
        registry.register_component<rectangle_t>();
        registry.register_component<circle_t>();
        registry.register_component<sprite_sheet_t>();
        registry.register_component<clickable_t>();
        registry.register_component<input_t>();
        registry.register_component<gravity_t>();
        registry.register_component<sound_t>();
        registry.register_component<behavior_t>();
        registry.register_component<snackbar_t>();
        registry.register_component<linked_t>();        
    }

    void init_nova_system(Registry& registry) {
        registry.add_rendering_system<position_t, rectangle_t, text_t, circle_t, sprite_t, sprite_sheet_t, input_t, sound_t, box_collider_t>(draw_system);
        registry.add_ui_system<clickable_t, box_collider_t, sprite_t, position_t, sound_t>(ui_system);
        registry.add_event_system<position_t, input_t>(input_system);
        registry.add_event_system<controllable_t>(keyboard_system);
        registry.add_standard_system<position_t, velocity_t, gravity_t>(position_system);
        registry.add_standard_system<velocity_t, controllable_t, acceleration_t>(controll_system);
        registry.add_standard_system<velocity_t, behavior_t, position_t, linked_t, actor_t>(behavior_system);
        registry.add_standard_system<position_t, box_collider_t, rigid_body_t, actor_t, weapon_t, score_t>(collide_detection_system);
        registry.add_standard_system<snackbar_t>(snack_system);
        behavior_t::lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string, sol::lib::table);
//        registry.add_standard_system<weapon_t, position_t>(delete_weapon_system);
        //registry.add_standard_system<actor_t, position_t>(delete_enemy_system);
    }
}