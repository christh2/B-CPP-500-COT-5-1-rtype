#include <iostream>
#include "../../../../../nova_engine/include/core/system.hpp"
#include "../../../../../nova_engine/include/core/entity.hpp"
#include "../../../../../nova_engine/include/core/hybrid_array.hpp"
#include "../../../../../nova_engine/include/core/register.hpp"
#include "../../../../include/core/game.hpp"
#include <map>

void add_main_player(NovaEngine::Registry& registry, std::size_t id, NovaEngine::position_t pos, std::string spaceship) {
    int y = id;
    registry.add_component<NovaEngine::position_t>(id, pos);
    if (spaceship == "BJ-551")
        registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("hero", 1, 0, 1, 0, y * 15, 33, 20, sf::Vector2f(2, 2)));
    else
        registry.add_component<NovaEngine::sprite_t>(id, NovaEngine::sprite_t(spaceship, sf::Vector2f(0.7, 0.7)));
    registry.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t());
    registry.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
    registry.add_component<NovaEngine::controllable_t>(id, NovaEngine::controllable_t());
    registry.add_entity(id);
}

void add_player(NovaEngine::Registry& registry, std::size_t id, NovaEngine::position_t pos) {
    int y = id;
    registry.add_component<NovaEngine::position_t>(id, pos);
    registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("hero", 1, 1, 1, 0, y * 15, 33, 20, sf::Vector2f(2, 2)));
    registry.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t());
    registry.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
    registry.add_entity(id);
}

void add_snack_bar(NovaEngine::Registry& registry, std::string message) {
    std::size_t id = registry.spawn_entity();
    std::size_t id2 = registry.spawn_entity();
    registry.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(600, 45));
    registry.add_component<NovaEngine::position_t>(id2, NovaEngine::position_t(660, 50));
    registry.add_component<NovaEngine::sprite_t>(id, NovaEngine::sprite_t("snack", sf::Vector2f(1, 1)));
    registry.add_component<NovaEngine::text_t>(id2, NovaEngine::text_t(message, "main_font", sf::Color(0, 0, 0), 20));    
    registry.add_entity(id);
    registry.add_entity(id2);
}

void manage_paralax(NovaEngine::Registry &registry) {
    NovaEngine::position_t &pos_0 = registry.get_component<NovaEngine::position_t>(0).value();
    NovaEngine::position_t &pos_1 = registry.get_component<NovaEngine::position_t>(1).value();
    NovaEngine::position_t &pos_2 = registry.get_component<NovaEngine::position_t>(2).value();

    if (pos_0._x < -2000) {
        pos_0._x = pos_2._x + 2000;
    } else if (pos_1._x < -2000) {
        pos_1._x = pos_0._x + 2000;
    } else if (pos_2._x < -2000) {
        pos_2._x = pos_1._x + 2000;
    }
}

void add_explosion(NovaEngine::Registry& registry, NovaEngine::position_t pos) {
    std::size_t id = registry.spawn_entity();
    registry.add_component<NovaEngine::position_t>(id, pos);
    registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("explosion", 5, 1, 200, 0, 95, 65, 69));
    registry.add_component<NovaEngine::snackbar_t>(id, NovaEngine::snackbar_t(800));
    registry.add_entity(id);
}

void other_player_attack(NovaEngine::Registry& registry, std::size_t id, NovaEngine::position_t pos) {
    registry.add_component<NovaEngine::position_t>(id, pos);
    registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("weapon", 0, 1, 1, 228, 102, 20, 13));
    registry.add_entity(id);
}

void bonus_attack(NovaEngine::Registry& registry, std::size_t id, NovaEngine::position_t pos, int category) {
    registry.add_component<NovaEngine::position_t>(id, pos);
    if (category == 0)
        registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("explode", 6, 0, 0.7, 0, 36, 49, 11, sf::Vector2f(2, 2)));
    else if (category == 1)
        registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("explode", 6, 0, 0.7, 0, 36, 49, 11, sf::Vector2f(2.5, 2.5)));
    else if (category == 2)
        registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("explode", 6, 0, 0.7, 0, 36, 49, 11, sf::Vector2f(3, 3)));
    else if (category == 3)
        registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("explode", 6, 0, 0.7, 0, 36, 49, 11, sf::Vector2f(3.2, 3.2)));
    registry.add_entity(id);
}

void ennemy_attack(NovaEngine::Registry& registry, std::size_t id, NovaEngine::position_t pos, int category) {
    registry.add_component<NovaEngine::position_t>(id, pos);
    if (category == 0)
        registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("weapon", 0, 1, 1, 256, 273, 18, 18));
    else if (category == 1)
        registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("weapon", 0, 1, 1, 157, 133, 61, 18));
    else if (category == 2)
        registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("weapon", 0, 1, 1, 130, 150, 73, 18));
    else if (category == 3)
        registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("weapon", 0, 1, 1, 93, 169, 91, 18));
    else if (category == 4)
        registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("weapon", 0, 1, 1, 93, 169, 91, 18, sf::Vector2f(2, 2)));
    registry.add_entity(id);
}

void bonus_pop_out(NovaEngine::Registry& registry, std::size_t id, NovaEngine::position_t pos) {
    registry.add_component<NovaEngine::position_t>(id, pos);
    registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("bonus", 0, 1, 1, 35, 12, 21, 21, sf::Vector2f(1.5, 1.5)));
    registry.add_entity(id);
}

void add_enemy(NovaEngine::Registry& registry, std::size_t id, NovaEngine::position_t pos, std::string asset)
{
    registry.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(pos._x, pos._y));
    if (asset == "Battle") {
        registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("Battle Ship", 8, 1, 200, 0, 0, 32, 32));
    } else if (asset == "First") {
        registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("Crevette", 1, 0, 200, 0, 1285, 180, 210, sf::Vector2f(3, 3)));
    } else if (asset == "Pique") {
        registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("Pique", 2, 1, 200, 285, 208, 66, 39, sf::Vector2f(1.7, 1.7)));
    } else if (asset == "Commandant") {
        registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("Commandant", 0, 1, 200, 0, 0, 50, 55, sf::Vector2f(3, 3)));
    } else if (asset == "Alien") {
        registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("Alien", 3, 1, 300, 0, 0, 32, 32, sf::Vector2f(3, 3)));
    } else if (asset == "Papillon") {
        registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("Boul", 7, 1, 200, 0, 0, 33, 33, sf::Vector2f(1.7, 1.7)));
    } else if (asset == "Robot") {
        registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("tenta", 7, 1, 200, 0, 0, 66, 130, sf::Vector2f(1.5, 1.5)));
    } else if (asset == "Second") {
        registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("gremlin", 3, 0, 200, 120, 0, 66, 82, sf::Vector2f(3, 3)));
    } else if (asset == "Third") {
        registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("worm", 1, 1, 200, 0, 0, 260, 139, sf::Vector2f(2, 2)));
    } else if (asset == "Fourth") {
        registry.add_component<NovaEngine::sprite_sheet_t>(id, NovaEngine::sprite_sheet_t("bosss", 1, 0, 0, 0, 0, 130, 50, sf::Vector2f(4, 4)));
    }
    registry.add_entity(id);
}

std::string to_asset(std::string enemy) {
    if (enemy == "Battle") {
        return "Battle";
    }
    if (enemy == "First") {
        return "Crevette";
    }
    if (enemy == "Pique") {
        return "Pique";
    }
    if (enemy == "Commandant") {
        return "Commandant";
    }
    if (enemy == "Alien") {
        return "Alien";
    }
    if (enemy == "Papillon") {
        return "Boul";
    }
    if (enemy == "Robot") {
        return "tenta";
    }
    if (enemy == "Second") {
        return "gremlin";
    }
    return "";
}

void update_enemy(NovaEngine::Registry& registry, std::size_t id, NovaEngine::position_t new_pos, std::string asset) {
    NovaEngine::hybrid_array<NovaEngine::position_t>& positions = registry.get_registry<NovaEngine::position_t>();
    NovaEngine::hybrid_array<NovaEngine::sprite_sheet_t>& sprites = registry.get_registry<NovaEngine::sprite_sheet_t>();
    NovaEngine::position_t &pos = positions[id].value();
    pos._x = new_pos._x;
    pos._y = new_pos._y;
    // NovaEngine::sprite_sheet_t &spr = sprites[id].value();
    // if (spr.asset_name != to_asset(asset)) {
    //     spr.asset_name = to_asset(asset);
    // }
}

void set_score(NovaEngine::Registry& registry, int score, int id) {
    std::size_t score_id = registry.spawn_entity();
    registry.add_component<NovaEngine::position_t>(score_id, NovaEngine::position_t(430, 200));
    registry.add_component<NovaEngine::text_t>(score_id, NovaEngine::text_t("Player " + std::to_string(id) + ": " + std::to_string(score), "main_font", sf::Color::White, 40));
    registry.add_entity(score_id);
}

void set_many_score(NovaEngine::Registry& registry, std::unordered_map<int, PlayerInfo> infos) {
    int y = 300;
    for (auto &it : infos) {
        std::size_t score_id = registry.spawn_entity();
        registry.add_component<NovaEngine::position_t>(score_id, NovaEngine::position_t(430, 200));
        registry.add_component<NovaEngine::text_t>(score_id, NovaEngine::text_t("Player " + std::to_string(it.second.id) + ": " + std::to_string(it.second.score), "main_font", sf::Color::White, 40));
        registry.add_entity(score_id);
        y += 100;
    }
}

void add_score(NovaEngine::Registry& registry, int x, int y, std::string text)
{
    std::size_t score_id = registry.spawn_entity();
    registry.add_component<NovaEngine::position_t>(score_id, NovaEngine::position_t(x, y));
    registry.add_component<NovaEngine::text_t>(score_id, NovaEngine::text_t(text, "main_font", sf::Color::White, 40));
    registry.add_entity(score_id);
}

void load_scores(NovaEngine::Registry& registry, nlohmann::json& data, std::string filepath)
{
    std::ifstream file(filepath);
    file >> data;
    int x = 500;
    int y = 400;
    for (const auto &it : data["scoreboard"]) {
        int i = it["score"];
        std::string name = it["name"];
        add_score(registry, x, y, name + ": " + std::to_string(i));
        y += 50;
    }
}

std::string get_key_name(sf::Keyboard::Key key) {
    static const std::unordered_map<sf::Keyboard::Key, std::string> keyNames = {
        {sf::Keyboard::A, "a"}, {sf::Keyboard::B, "b"}, {sf::Keyboard::C, "c"},
        {sf::Keyboard::D, "d"}, {sf::Keyboard::E, "e"}, {sf::Keyboard::F, "f"},
        {sf::Keyboard::G, "g"}, {sf::Keyboard::H, "h"}, {sf::Keyboard::I, "i"},
        {sf::Keyboard::J, "j"}, {sf::Keyboard::K, "k"}, {sf::Keyboard::L, "l"},
        {sf::Keyboard::M, "m"}, {sf::Keyboard::N, "n"}, {sf::Keyboard::O, "o"},
        {sf::Keyboard::P, "p"}, {sf::Keyboard::Q, "q"}, {sf::Keyboard::R, "r"},
        {sf::Keyboard::S, "s"}, {sf::Keyboard::T, "t"}, {sf::Keyboard::U, "u"},
        {sf::Keyboard::V, "v"}, {sf::Keyboard::W, "w"}, {sf::Keyboard::X, "x"},
        {sf::Keyboard::Y, "y"}, {sf::Keyboard::Z, "z"},
        {sf::Keyboard::Num0, "0"}, {sf::Keyboard::Num1, "1"},
        {sf::Keyboard::Num2, "2"}, {sf::Keyboard::Num3, "3"},
        {sf::Keyboard::Num4, "4"}, {sf::Keyboard::Num5, "5"},
        {sf::Keyboard::Num6, "6"}, {sf::Keyboard::Num7, "7"},
        {sf::Keyboard::Num8, "8"}, {sf::Keyboard::Num9, "9"},
        {sf::Keyboard::Escape, "escape"}, {sf::Keyboard::LControl, "left control"},
        {sf::Keyboard::LShift, "left shift"}, {sf::Keyboard::LAlt, "left alt"},
        {sf::Keyboard::LSystem, "left system"}, {sf::Keyboard::RControl, "right control"},
        {sf::Keyboard::RShift, "right shift"}, {sf::Keyboard::RAlt, "right alt"},
        {sf::Keyboard::RSystem, "right system"}, {sf::Keyboard::Menu, "menu"},
        {sf::Keyboard::LBracket, "["}, {sf::Keyboard::RBracket, "]"},
        {sf::Keyboard::Semicolon, ";"}, {sf::Keyboard::Comma, ","},
        {sf::Keyboard::Period, "."}, {sf::Keyboard::Quote, "'"},
        {sf::Keyboard::Slash, "/"}, {sf::Keyboard::Backslash, "\\"},
        {sf::Keyboard::Tilde, "~"}, {sf::Keyboard::Equal, "="},
        {sf::Keyboard::Hyphen, "-"}, {sf::Keyboard::Space, "space"},
        {sf::Keyboard::Enter, "enter"}, {sf::Keyboard::Backspace, "backspace"},
        {sf::Keyboard::Tab, "tab"}, {sf::Keyboard::PageUp, "page up"},
        {sf::Keyboard::PageDown, "page down"}, {sf::Keyboard::End, "end"},
        {sf::Keyboard::Home, "home"}, {sf::Keyboard::Insert, "insert"},
        {sf::Keyboard::Delete, "delete"}, {sf::Keyboard::Add, "numpad +"},
        {sf::Keyboard::Subtract, "numpad -"}, {sf::Keyboard::Multiply, "numpad *"},
        {sf::Keyboard::Divide, "numpad /"}, {sf::Keyboard::Left, "left"},
        {sf::Keyboard::Right, "right"}, {sf::Keyboard::Up, "up"},
        {sf::Keyboard::Down, "down"}, {sf::Keyboard::Numpad0, "numpad 0"},
        {sf::Keyboard::Numpad1, "numpad 1"}, {sf::Keyboard::Numpad2, "numpad 2"},
        {sf::Keyboard::Numpad3, "numpad 3"}, {sf::Keyboard::Numpad4, "numpad 4"},
        {sf::Keyboard::Numpad5, "numpad 5"}, {sf::Keyboard::Numpad6, "numpad 6"},
        {sf::Keyboard::Numpad7, "numpad 7"}, {sf::Keyboard::Numpad8, "numpad 8"},
        {sf::Keyboard::Numpad9, "numpad 9"}, {sf::Keyboard::F1, "f1"},
        {sf::Keyboard::F2, "f2"}, {sf::Keyboard::F3, "f3"}, {sf::Keyboard::F4, "f4"},
        {sf::Keyboard::F5, "f5"}, {sf::Keyboard::F6, "f6"}, {sf::Keyboard::F7, "f7"},
        {sf::Keyboard::F8, "f8"}, {sf::Keyboard::F9, "f9"}, {sf::Keyboard::F10, "f10"},
        {sf::Keyboard::F11, "f11"}, {sf::Keyboard::F12, "f12"}, {sf::Keyboard::Pause, "pause"}
    };

    auto it = keyNames.find(key);
    if (it != keyNames.end()) {
        return it->second;
    }
    return "Unknown";
}
