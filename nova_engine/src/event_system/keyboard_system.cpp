#include <iostream>
#include "../../include/core/system.hpp"
#include "../../include/core/entity.hpp"
#include "../../include/core/hybrid_array.hpp"

namespace NovaEngine {
    void keyboard_system(std::vector<Entity> entities, message_t &message_queue, sf::RenderWindow& window, sf::Event& event,
    hybrid_array<controllable_t>& controllables) {
        for (auto &i : entities) {
            auto &control = controllables[i];
            if (control.has_value() && control.value().is_controllable) {
                controllable_t &control_vlue = control.value();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                    control_vlue.control = "R";
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                    control_vlue.control = "L";
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                    control_vlue.control = "U";
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                    control_vlue.control = "D";
                }
            }
        }
    }
}
