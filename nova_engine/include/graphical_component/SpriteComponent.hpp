#include <iostream>
#include <SFML/Graphics.hpp>
#include <memory>
#include <list>
#include <filesystem>

namespace NovaEngine {
      typedef struct sprite_t {
        std::string asset_name;
        sf::Vector2f size;
        sprite_t(const std::string name, sf::Vector2f size) : asset_name(name), size(size) {}
      } sprite_t;

      typedef struct sprite_sheet_t {
        std::string asset_name;
        sf::IntRect rect;
        int nb_col, nb_row, current_column, current_row;
        double _frame_duration;
        sf::Vector2f size;
        sf::Clock clk;
        sf::Time time;
        bool init;
        sprite_sheet_t(const std::string& asset_name, int col, int row,
         double frame_duration, int x, int y, int w, int h,sf::Vector2f size = sf::Vector2f(1, 1)) {
            this->asset_name = asset_name;
            this->nb_col = col;
            this->nb_row = row;
            this->init = false;
            this->_frame_duration = frame_duration;
            this->current_column = x;
            this->current_row = y;
            this->size = size;
            this->rect = sf::IntRect(x, y, w, h);
        }

        void update() {
          if (nb_col == 0 || nb_row == 0)
              return;
          if (!this->init) {
              this->clk.restart();
              this->init = true;
          }
          this->time = this->clk.getElapsedTime();
          if (this->time.asMilliseconds() > _frame_duration) {
              current_column += rect.width;
              if (current_column >= ((nb_col+1) * rect.width)) {
                current_column = 0;
                current_row += rect.height;
                if (current_row >= ((nb_row) * rect.height)) {
                  current_row = 0;
                }
              }
              this->clk.restart();
              this->rect = sf::IntRect(current_column, current_row, rect.width, rect.height);
          }
        }
  } sprite_sheet_t;
}