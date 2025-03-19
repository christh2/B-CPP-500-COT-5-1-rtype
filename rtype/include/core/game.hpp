#pragma once
#include "../../../nova_engine/include/core/system.hpp"
#include "../../../nova_engine/include/core/register.hpp"
#include "../network/client/client.hpp"
#include <nlohmann/json.hpp>
#include <fstream>

//Personal system
namespace NovaEngine {
    std::string ui_system(std::vector<Entity> entities, sf::RenderWindow &window, sf::Event event,
        hybrid_array<clickable_t>& clickables, hybrid_array<box_collider_t>& boxes,
        hybrid_array<sprite_t>& sprites, hybrid_array<position_t>& positions, hybrid_array<sound_t>& sounds);
}

//Init Nova engine
namespace NovaEngine {
    void init_nova_component(Registry& registry);
    void init_nova_system(Registry& registry);
    void delete_weapon_system(std::vector<Entity> entities, message_t &message_queue, hybrid_array<weapon_t>& weapons,
        hybrid_array<position_t>& positions);
}

typedef struct user {
    int money;
    std::vector<std::string> skins;
} user_t;

typedef enum path {
    MENU,
    OPTIONS,
    MODE,
    GAME_OVER,
    LOGIN,
    SPACESHIP,
    ROOM,
    ROOM_INFO,
    PLAY,
    SURVIVAL,
    HISTORY,
    SCOREBOARD,
    LOADING,
    QUIT
} path_t;

void update_enemy(NovaEngine::Registry& registry, std::size_t id, NovaEngine::position_t new_pos, std::string asset);
void add_player(NovaEngine::Registry& registry, std::size_t id, NovaEngine::position_t pos);
void add_main_player(NovaEngine::Registry& registry, std::size_t id, NovaEngine::position_t pos, std::string spaceship = "BJ-551");
void other_player_attack(NovaEngine::Registry& registry, std::size_t id, NovaEngine::position_t pos);
void add_snack_bar(NovaEngine::Registry& registry, std::string message);
void set_score(NovaEngine::Registry& registry, int score, int id);
void handle_attack_request(NovaEngine::Registry& registry);
void add_enemy(NovaEngine::Registry& registry, std::size_t id, NovaEngine::position_t pos, std::string asset);
std::string get_key_name(sf::Keyboard::Key key);
void load_scores(NovaEngine::Registry& registry, nlohmann::json& data, std::string filepath);
void add_explosion(NovaEngine::Registry& registry, NovaEngine::position_t pos);
void set_many_score(NovaEngine::Registry& registry, std::unordered_map<int, PlayerInfo> infos);
void spawn_ennemy(NovaEngine::Registry &registry, std::string name, int level);
void manage_paralax(NovaEngine::Registry &registry);
void ennemy_attack(NovaEngine::Registry& registry, std::size_t id, NovaEngine::position_t pos, int category);
void bonus_pop_out(NovaEngine::Registry& registry, std::size_t id, NovaEngine::position_t pos);
void add_survival_player(NovaEngine::Registry& registry, std::size_t id, NovaEngine::position_t pos, std::string skin);
void bonus_attack(NovaEngine::Registry& registry, std::size_t id, NovaEngine::position_t pos, int category);
