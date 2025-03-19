#include <iostream>
#include <SFML/Graphics.hpp>

typedef struct button_t {
  public:
    int width, height;
    std::string button_text;
    sf::Color color;
    button_t(int width, int height, std::string button_text, sf::Color color = sf::Color(0, 0, 0)) : width(width), height(height), button_text(button_text), color(color) {}
    ~button_t(){};
} button_t;