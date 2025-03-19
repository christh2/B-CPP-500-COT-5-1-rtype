#include <iostream>
#include <SFML/Graphics.hpp>

namespace NovaEngine {
  typedef struct rectangle_t {
    public:
      double width, height;
      sf::Color color;
      bool is_filled;
      rectangle_t(double width, double height, sf::Color color, bool is_filled) : width(width), height(height), color(color), is_filled(is_filled) {};
      ~rectangle_t(){};
  } rectangle_t;
}
