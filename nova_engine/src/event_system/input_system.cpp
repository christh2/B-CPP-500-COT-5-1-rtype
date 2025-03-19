#include <iostream>
#include "../../include/core/system.hpp"
#include "../../include/core/entity.hpp"
#include "../../include/core/hybrid_array.hpp"

namespace NovaEngine {
    bool valid_char(char c) {
        if (c >= 'a' && c <= 'z')
            return true;
        if (c >= 'A' && c <= 'Z')
            return true;
        if (c >= '0' && c <= '9')
            return true;
        if (c == '-' || c == ' ' || c == '.')
            return true;
        return false;
    }

    void process_char(input_t &input, char c) {
        if (c == '\b' && !input.input.empty()) {
            input.input.pop_back();
        } else if (valid_char(c) && input.input.size() < 15) {
            input.input.push_back(c);
        }
    }

    void process_select(input_t &input, sf::Vector2i mouse_pos, position_t pos) {
        if (mouse_pos.x >= pos._x && mouse_pos.x <= (pos._x + input.width) &&
            mouse_pos.y >= pos._y && mouse_pos.y <= (pos._y + input.height)) {
                input.selected = true;
        } else {
            input.selected = false;
        }
    }

    void input_system(std::vector<Entity> entities, message_t &message_queue, sf::RenderWindow& window, sf::Event &event,
    hybrid_array<position_t>& positions, hybrid_array<input_t>& inputs) {
        for (auto& entity : entities) {
            auto& position = positions[entity];
            auto& input = inputs[entity];
            if (input.has_value() && position.has_value()) {
                input_t& input_vlue = input.value();
                sf::Vector2i mouse_pos = sf::Mouse::getPosition(window);
                position_t &pos = position.value();
                if (event.type == sf::Event::TextEntered && input_vlue.selected) {
                    process_char(input_vlue, static_cast<char>(event.text.unicode));
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    process_select(input_vlue, mouse_pos, pos);
                }
            }
        }
    }
}
