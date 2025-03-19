#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

namespace NovaEngine {
    typedef struct input_t {
        std::string input;
        std::string default_text;
        std::string font;
        bool selected;
        int width, height, border_length;
        sf::Color border_color;
        sf::Color text_color;
        input_t(std::string default_text, int width, int height, std::string font = "main_font", bool selected = false) : default_text(default_text), font(font),
            width(width), height(height), selected(selected) {}
    } input_t;
}
