#include <iostream>
#include "../../include/core/register.hpp"
#include "../../include/core/system.hpp"
#include "../../include/core/system.hpp"

namespace NovaEngine {
    void draw_system(std::vector<Entity> entities, message_t &message_queue, sf::RenderWindow &window, Asset_handler &handler,
    hybrid_array<position_t>& positions, hybrid_array<rectangle_t>& rectangles, hybrid_array<text_t> texts,
    hybrid_array<circle_t>& circles, hybrid_array<sprite_t> sprite, hybrid_array<sprite_sheet_t> &anim_sprite,
    hybrid_array<input_t>& inputs, hybrid_array<sound_t>& sounds, hybrid_array<box_collider_t>& boxes) {
        sf::RectangleShape rect;
        sf::CircleShape circle;
        sf::Text text;
        for (auto &i : entities) {
            auto &pos = positions[i];
            auto &rectangle = rectangles[i];
            auto &text_info = texts[i];
            auto &circle_info = circles[i];
            auto &sprite_info = sprite[i];
            auto &anim_sprite_info = anim_sprite[i];
            auto &input_info = inputs[i];
            auto &sound_info = sounds[i];
            auto &box = boxes[i];
            if (pos.has_value()) {
                if (rectangle.has_value()) {
                    rect.setPosition(pos.value()._x, pos.value()._y);
                    rect.setSize(sf::Vector2f(rectangle.value().width, rectangle.value().height));
                    if (rectangle.value().is_filled) {
                        rect.setFillColor(rectangle.value().color);
                    } else {
                        rect.setOutlineColor(rectangle.value().color);
                        rect.setFillColor(sf::Color::Transparent);
                        rect.setOutlineThickness(4);
                    }
                    window.draw(rect);
                }
                if (circle_info.has_value()) {
                    circle.setPosition(pos.value()._x, pos.value()._y);
                    circle.setRadius(circle_info.value().radius);
                    circle.setFillColor(circle_info.value().color);
                    circle.setOutlineColor(circle_info.value().color);
                    window.draw(circle);
                }
                if (text_info.has_value()) {
                    text.setPosition(pos.value()._x, pos.value()._y);
                    text.setCharacterSize(text_info.value().character_size);
                    text.setFillColor(text_info.value().color);
                    text.setString(text_info.value().text);
                    handler.set_font(text_info.value().font_id, text);
                    window.draw(text);
                }
                if (sprite_info.has_value()) {
                    handler.draw_asset(window, sprite_info.value().asset_name, sprite_info.value().size, sf::Vector2f(pos.value()._x, pos.value()._y));
                }
                if (anim_sprite_info.has_value()) {
                    NovaEngine::sprite_sheet_t &sprite_sheet = anim_sprite_info.value();
                    sprite_sheet.update();
                    handler.draw_asset(window, anim_sprite_info.value().asset_name, anim_sprite_info.value().rect, anim_sprite_info.value().size,
                        sf::Vector2f(pos.value()._x, pos.value()._y));
                    // if (box.has_value()) {
                    //     rect.setPosition(pos.value()._x, pos.value()._y);
                    //     rect.setSize(sf::Vector2f(box.value().width, box.value().height));
                    //     rect.setFillColor(sf::Color::Transparent);
                    //     rect.setOutlineColor(sf::Color::White);
                    //     rect.setOutlineThickness(4);
                    //     window.draw(rect);
                    // }
                }
                if (sound_info.has_value() && sound_info.value().can_play) {
                    sound_t &sound = sound_info.value();
                    handler.play_music(sound.sound_file, sound.volume, sound.loop);
                }
                if (input_info.has_value()) {
                    input_t& input_vlue = input_info.value();
                    sf::Font font;
                    rect.setPosition(pos.value()._x, pos.value()._y);
                    rect.setSize(sf::Vector2f(input_vlue.width, input_vlue.height));
                    rect.setFillColor(sf::Color::Transparent);
                    rect.setOutlineColor(sf::Color::White);
                    rect.setOutlineThickness(4);
                    window.draw(rect);
                    text.setPosition(pos.value()._x + 10, pos.value()._y + 10);
                    text.setCharacterSize(20);
                    text.setFillColor(sf::Color::White);
                    text.setString(input_vlue.input.empty() ? input_vlue.default_text : input_vlue.input);
                    font.loadFromFile(handler.get_font_path(input_vlue.font));
                    text.setFont(font);
                    window.draw(text);
                }
            }
        }
    }
}
