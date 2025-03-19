#include "zuma_utils.hpp"
#include "engine_init.hpp"

class Zuma {
    public:
        Zuma() : window(sf::VideoMode(1900, 1000), "Zuma") {
            path = MENU;
            for (int i = 0; i < 7; i++) {
                NovaEngine::Registry tmp;
                NovaEngine::init_nova_component(tmp);
                NovaEngine::init_nova_system(tmp);
                my_game.push_back(tmp);
            }
            NovaEngine::init_asset(handler, "../assets/zuma_config/assets.json");
            NovaEngine::init_scene(my_game[MENU], "../assets/zuma_config/menu.json");
        }

        ~Zuma() {}

        void kill_dead_entities() {
            NovaEngine::Registry& reg = my_game[GAME];
            NovaEngine::hybrid_array<NovaEngine::weapon_t>& weapons = my_game[GAME].get_registry<NovaEngine::weapon_t>();
            for (auto& entity : reg.get_entities()) {
                auto& weapon = weapons[entity];
                if (weapon.has_value() && weapon.value().lives == 0) {
                    reg.kill_entity(entity);
                }
            }
        }

        void proccess_ui(std::string compo) {
            if (compo == "Quit") {
                path = QUIT;
            } else if (compo == "GAME") {
                path = GAME;
            } else if (compo == "MENU") {
                path = MENU;
            } else if (compo == "OPTIONS") {
                path = OPTIONS;
            } else if (compo == "SHOP") {
                path = SHOP;
            } else if (compo == "LEVELS") {
                path = LEVELS;
            } else if (compo == "CHOICE") {
                path = CHOICE;
            }
        }

        void run() {
            std::string compo;
            while (window.isOpen()) {
                sf::Event event;
                while (window.pollEvent(event)) {
                    my_game[path].run_event_system(event, window, handler);
                    if (event.type == sf::Event::Closed) {
                        window.close();
                    }
                }
                if (path == QUIT) {
                    window.close();
                }
                window.clear();
                my_game[path].run_rendering_system(window, handler);
                compo = my_game[path].run_ui_system(window, event);
                window.display();
                if (path != QUIT) {
                    my_game[path].run_standard_system();
                }
                proccess_ui(compo);
            }
        }

    private:
        sf::RenderWindow window;
        NovaEngine::Asset_handler handler;
        path_t path;
        std::vector<NovaEngine::Registry> my_game;
};
