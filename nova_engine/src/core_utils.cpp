#include "../include/core/core_utils.hpp"

void NovaEngine::init_asset(NovaEngine::Asset_handler& handler, std::string asset_path) {
    std::map<std::string,std::size_t> doc;
    std::ifstream file(asset_path);
    nlohmann::json scene;
    file >> scene;
    for (const auto &it : scene["assets"]) {
        handler.set_asset(it["name"], it["path"]);
    }
    for (const auto &it : scene["multi_frame_assets"]) {
        handler.set_multi_frame_asset(it["name"], it["path"], it["frame"], it["once"]);
    }
    for (const auto &it : scene["fonts"]) {
        handler.set_font(it["name"], it["path"]);
    }
    for (const auto &it : scene["musics"]) {
        handler.set_music_file(it["name"], it["path"]);
    }
    for (const auto &it : scene["audios"]) {
        handler.set_audio(it["name"], it["path"]);
    }
}

void NovaEngine::init_scene(NovaEngine::Registry& registry, std::string file_path) {
    std::map<std::string,std::size_t> doc;
    std::ifstream file(file_path);
    nlohmann::json scene;
    file >> scene;
    for (const auto &it : scene["entities"]) {
        doc[it] = registry.spawn_entity();
    }
    for (const auto &it : scene["sprites"]) {
        registry.add_component<NovaEngine::sprite_t>(doc[it["entity"]], NovaEngine::sprite_t(it["asset"], sf::Vector2f(it["scale"]["x"], it["scale"]["y"])));
    }
    for (const auto &it : scene["spritesheets"]) {
        registry.add_component<NovaEngine::sprite_sheet_t>(doc[it["entity"]], NovaEngine::sprite_sheet_t(it["asset"], it["nb_col"], it["nb_row"], it["frame"], it["x"], it["y"], it["w"], it["h"], sf::Vector2f(it["scale"]["x"], it["scale"]["y"])));
    }
    for (const auto &it : scene["texts"]) {
        registry.add_component<NovaEngine::text_t>(doc[it["entity"]], NovaEngine::text_t(it["content"], it["font"],sf::Color(it["color"]["r"], it["color"]["g"], it["color"]["b"]), it["size"]));
    }
    for (const auto &it : scene["positions"]) {
        registry.add_component<NovaEngine::position_t>(doc[it["entity"]], NovaEngine::position_t(it["x"], it["y"]));
    }
    for (const auto &it : scene["clickables"]) {
        registry.add_component<NovaEngine::clickable_t>(doc[it["entity"]], NovaEngine::clickable_t(it["name"]));
    }
    for (const auto &it : scene["colliders"]) {
        registry.add_component<NovaEngine::box_collider_t>(doc[it["entity"]], NovaEngine::box_collider_t(it["width"], it["height"]));
    }
    for (const auto &it : scene["sounds"]) {
        registry.add_component<NovaEngine::sound_t>(doc[it["entity"]], NovaEngine::sound_t(it["name"], it["volume"], it["loop"]));
    }
    for (const auto &it : scene["inputs"]) {
        registry.add_component<NovaEngine::input_t>(doc[it["entity"]], NovaEngine::input_t(it["name"], it["width"], it["height"], it["font"]));
    }
    for (const auto &it : scene["rectangles"]) {
        registry.add_component<NovaEngine::rectangle_t>(doc[it["entity"]], NovaEngine::rectangle_t(it["width"], it["height"], sf::Color(it["color"]["r"], it["color"]["g"], it["color"]["b"]), it["fill"]));
    }
    for (const auto &it : scene["circles"]) {
        registry.add_component<NovaEngine::circle_t>(doc[it["entity"]], NovaEngine::circle_t(it["radius"], it["filled"], sf::Color(it["color"]["r"], it["color"]["g"], it["color"]["b"])));
    }
    for (const auto &it : scene["velocities"]) {
        registry.add_component<NovaEngine::velocity_t>(doc[it["entity"]], NovaEngine::velocity_t(it["x"], it["y"], it["dir_x"], it["dir_y"]));
    }
    for (const auto &it : scene["snacks"]) {
        registry.add_component<NovaEngine::snackbar_t>(doc[it["entity"]], NovaEngine::snackbar_t(it["duration"]));
    }
}
