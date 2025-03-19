#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

namespace NovaEngine {
    typedef struct clickable_t {
        std::string ui_compo;
        bool clickable;
        clickable_t(std::string ui_compo, bool clickable = true) : ui_compo(ui_compo), clickable(clickable) {}
    } clickable_t;
}
