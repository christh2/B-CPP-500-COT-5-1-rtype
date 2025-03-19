#include <iostream>
#include <SFML/Graphics.hpp>

namespace NovaEngine {
  typedef struct circle_t {
    public:
      double radius;
      bool filled;
      sf::Color color;
      circle_t(double radius, bool filled, sf::Color color = sf::Color(0, 0, 0)) : radius(radius), filled(filled), color(color) {};
      ~circle_t(){};
  } circle_t;
}
