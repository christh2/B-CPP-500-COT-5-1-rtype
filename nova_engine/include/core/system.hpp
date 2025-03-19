#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "entity.hpp"
#include "hybrid_array.hpp"
#include "../event_controller/controllable_component.hpp"
#include "../event_controller/clickable_component.hpp"
#include "../event_controller/input_component.hpp"
#include "../ui_component/drawable_component.hpp"
#include "../physics_component/position_component.hpp"
#include "../physics_component/velocity_component.hpp"
#include "../physics_component/gravity_component.hpp"
#include "../physics_component/force_component.hpp"
#include "../physics_component/ground_component.hpp"
#include "../physics_component/acceleration_component.hpp"
#include "../physics_component/mass_component.hpp"
#include "../physics_component/rectangle_box_component.hpp"
#include "../physics_component/rigid_body_component.hpp"
#include "../graphical_component/CircleComponent.hpp"
#include "../graphical_component/RectangleComponent.hpp"
#include "../graphical_component/SpriteComponent.hpp"
#include "../graphical_component/TextComponent.hpp"
#include "../role_component/actor_component.hpp"
#include "../role_component/score_component.hpp"
#include "../role_component/snack_bar_component.hpp"
#include "../role_component/behavioral_component.hpp"
#include "../role_component/linker_component.hpp"
#include "../audio_component/soundComponent.hpp"
#include "message.hpp"
#include "asset_handler.hpp"

namespace NovaEngine {
    void controll_system(std::vector<Entity> entities, message_t &message_queue, hybrid_array<velocity_t>& velocities,
    hybrid_array<controllable_t>& controllables, hybrid_array<acceleration_t>& accelerations);
    void keyboard_system(std::vector<Entity> entities, message_t &message_queue, sf::RenderWindow& window, sf::Event& event,
    hybrid_array<controllable_t>& controllables);
    void draw_system(std::vector<Entity> entities, message_t &message_queue, sf::RenderWindow &window, Asset_handler &handler,
        hybrid_array<position_t>& positions, hybrid_array<rectangle_t>& rectangles, hybrid_array<text_t> texts,
        hybrid_array<circle_t>& circles, hybrid_array<sprite_t> sprite, hybrid_array<sprite_sheet_t> &anim_sprite,
        hybrid_array<input_t>& inputs, hybrid_array<sound_t>& sounds, hybrid_array<box_collider_t>& boxes);
    void collide_detection_system(std::vector<Entity> entities, message_t &message_queue, hybrid_array<position_t>& positions,
        hybrid_array<box_collider_t>& collider, hybrid_array<rigid_body_t>& bodies,
        hybrid_array<actor_t>& actors, hybrid_array<weapon_t>& weapons, hybrid_array<score_t>& scores);
    void collision_response_system(std::vector<Entity> entities, message_t &message_queue, hybrid_array<rigid_body_t>& positions, hybrid_array<score_t>& scores,
        hybrid_array<actor_t>& actors, hybrid_array<weapon_t>& weapons);
    void position_system(std::vector<Entity> entities, message_t &message_queue, hybrid_array<position_t>& positions, hybrid_array<velocity_t>& velocities, hybrid_array<gravity_t>& gravities);
    void input_system(std::vector<Entity> entities, message_t &message_queue, sf::RenderWindow& window, sf::Event& event,
        hybrid_array<position_t>& positions, hybrid_array<input_t>& inputs);
    std::string ui_system(std::vector<Entity> entities, sf::RenderWindow &window, sf::Event event,
        hybrid_array<clickable_t>& clickables, hybrid_array<box_collider_t>& boxes,
        hybrid_array<sprite_t>& sprites, hybrid_array<position_t>& positions, hybrid_array<sound_t>& sounds);
    void delete_enemy_system(std::vector<Entity> entities, message_t &message_queue, hybrid_array<actor_t>& actors,
        hybrid_array<position_t>& positions);
    void snack_system(std::vector<Entity> entities, message_t &message_queue, hybrid_array<snackbar_t>& snacks);
    void behavior_system(std::vector<Entity> entities, message_t &message_queue, hybrid_array<velocity_t>& velocities,
    hybrid_array<behavior_t>& scripts, hybrid_array<position_t>& positions, hybrid_array<linked_t>& links,
    hybrid_array<actor_t>& actors);
}
