#include <iostream>
#include "../../include/core/system.hpp"
#include "../../include/core/entity.hpp"
#include "../../include/core/hybrid_array.hpp"

namespace NovaEngine {
    std::string ui_system(std::vector<Entity> entities, sf::RenderWindow &window, sf::Event event,
    hybrid_array<clickable_t>& clickables, hybrid_array<box_collider_t>& boxes,
    hybrid_array<sprite_t>& sprites, hybrid_array<position_t>& positions, hybrid_array<sound_t>& sounds) {
        for (auto &i : entities) {
            auto& clickable = clickables[i];
            auto& box = boxes[i];
            auto& sprite = sprites[i];
            auto& position = positions[i];
            auto& sound = sounds[i];
            if (clickable.has_value() && clickable.value().clickable && box.has_value() && position.has_value()) {
                sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                box_collider_t &box_vlue = box.value();
                position_t pos = position.value();
                if (mouse_pos.x >= pos._x && mouse_pos.x <= (pos._x + box_vlue.width) &&
                    mouse_pos.y >= pos._y && mouse_pos.y <= pos._y + box_vlue.height) {
                        if (sprite.has_value()) {
                            sprite_t &spr = sprite.value();
                            if (spr.asset_name.find("hover") == std::string::npos)
                                spr.asset_name = spr.asset_name + " hover";
                        } if (event.type == sf::Event::MouseButtonPressed) {
                            return clickable.value().ui_compo;
                        }
                } else {
                    if (sprite.has_value()) {
                        sprite_t &spr = sprite.value();
                        if (spr.asset_name.find("hover") != std::string::npos)
                            spr.asset_name = spr.asset_name.substr(0, spr.asset_name.size() - 6);
                    } if (sound.has_value()) {
                        sound_t &snd = sound.value();
                        snd.can_play = false;
                    }
                }
            }
        }
        return "";
    }
}