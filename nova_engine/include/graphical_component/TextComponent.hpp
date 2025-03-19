#include <iostream>
#include <SFML/Graphics.hpp>

namespace NovaEngine {
  typedef struct text_t {
    std::string text;
    std::string font_id;
    int character_size;
    sf::Color color;
    text_t(std::string text, std::string font, sf::Color color, int char_size = 30) : text(text), font_id(font), color(color), character_size(char_size) {}
    ~text_t() {}
  } text_t;
}
