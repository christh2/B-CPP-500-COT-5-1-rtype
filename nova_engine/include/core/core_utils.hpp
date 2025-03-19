#include <iostream>
#include "register.hpp"
#include "system.hpp"
#include "asset_handler.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <map>

namespace NovaEngine {
    void init_asset(Asset_handler& handler, std::string asset_path);
    void init_scene(Registry& registry, std::string file_path);
}