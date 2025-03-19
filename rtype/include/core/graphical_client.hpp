#include "../../../nova_engine/include/core/register.hpp"
#include "../../../nova_engine/include/core/asset_handler.hpp"
#include "../../../nova_engine/include/core/entity.hpp"
#include "../../../nova_engine/include/core/core_utils.hpp"
#include "../network/client/client.hpp"
#include "game.hpp"
#include <cstdlib>
#include <queue>

std::queue<sf::Image> frameQueue;
std::mutex queueMutex;
std::condition_variable frame_condition;
std::atomic<bool> stop_thread = false;
std::string user = "default";

std::string frame_getter(int count)
{
    if (count < 10) {
        return "0000"+std::to_string(count);
    } else if (count < 100) {
        return "000"+std::to_string(count);
    } else if (count < 1000) {
        return "00"+std::to_string(count);
    } else if (count < 10000) {
        return "0"+std::to_string(count);
    }
    return std::to_string(count);
}

void saveFrames() {
    int frameCounter = 0;

    while (!stop_thread) {
        std::unique_lock<std::mutex> lock(queueMutex);
        frame_condition.wait(lock, [] { return !frameQueue.empty() || stop_thread; });
        while (!frameQueue.empty()) {
            sf::Image frame = frameQueue.front();
            frameQueue.pop();
            lock.unlock();
            if (frame.getSize().x > 0 && frame.getSize().y > 0) {
                std::string path = "frame_" + frame_getter(frameCounter++) + ".png";
                std::string cmd = "mv " + path + " ../records/"+user;
                frame.saveToFile(path);
                system(cmd.c_str());
            } else {
                std::cerr << "Invalid frame data!" << std::endl;
            }
            lock.lock();
        }
    }
    std::string command = "ffmpeg -framerate 20 -i ../records/"+user+"/frame_%05d.png -c:v libx264 -pix_fmt yuv420p output_video.mp4 2> error.log";
    std::string command_2 = "mv output_video.mp4 ../exports/"+user+" 2>/dev/null";
    int result = system(command.c_str());
    system(command_2.c_str());
    std::cout << result << std::endl;
}

class Graphical_client {
    public:
        Graphical_client(std::unique_ptr<NovaEngine::Asset_handler> &handle) : window(sf::VideoMode(1900, 1000), "R TYPE") {
            path = MENU;
            history.push_back(path);
            volume = 70;
            skin_index = 0;
            bind_selector = "";
            select = 0;
            select_2 = 0;
            level = 0;
            weapons = "basic_weapon";
            skins = {"rocket", "purple", "shadow", "monolith", "nexus", "BJ-551"};
            handler = std::move(handle);
            start = false;
            can_play = false;
            record = false;
            inputBindings = {
                {"up", sf::Keyboard::Up},
                {"down", sf::Keyboard::Down},
                {"left", sf::Keyboard::Left},
                {"right", sf::Keyboard::Right},
                {"auto", sf::Keyboard::Space},
                {"shoot", sf::Keyboard::A}
            };
            for (int i = 0; i <= 12; i++) {
                NovaEngine::Registry tmp;
                NovaEngine::init_nova_component(tmp);
                NovaEngine::init_nova_system(tmp);
                my_game.push_back(tmp);
            }
            levels = {"Milky way", "Andromed Room", "Scorpion center", "Holy Departure"};
            NovaEngine::init_scene(my_game[MENU], "../assets/rtype_config/start.json");
            NovaEngine::init_scene(my_game[PLAY], "../assets/rtype_config/players.json");
            NovaEngine::init_scene(my_game[OPTIONS], "../assets/rtype_config/option.json");
            NovaEngine::init_scene(my_game[HISTORY], "../assets/rtype_config/history.json");
            NovaEngine::init_scene(my_game[LOGIN], "../assets/rtype_config/login.json");
            NovaEngine::init_scene(my_game[SPACESHIP], "../assets/rtype_config/spaceships.json");
            NovaEngine::init_scene(my_game[SURVIVAL], "../assets/rtype_config/endless.json");
            NovaEngine::init_scene(my_game[MODE], "../assets/rtype_config/game_mode.json");
            NovaEngine::init_scene(my_game[ROOM_INFO], "../assets/rtype_config/room.json");
            NovaEngine::init_scene(my_game[LOADING], "../assets/rtype_config/loading.json");
            NovaEngine::init_scene(my_game[GAME_OVER], "../assets/rtype_config/game_over.json");
            NovaEngine::init_scene(my_game[SCOREBOARD], "../assets/rtype_config/score.json");
            load_scores(my_game[SCOREBOARD], scores, "../assets/.save/scoreboard.json");
            level = 0;
            clock.restart();
        }

        ~Graphical_client() {}

        void get_and_send(boost::asio::io_context& io_context) {
            std::string usr_name;
            std::string ip;
            NovaEngine::hybrid_array<NovaEngine::input_t>& inputs = my_game[LOGIN].get_registry<NovaEngine::input_t>();
            for (auto& entity : my_game[LOGIN].get_entities()) {
                auto& input = inputs[entity];
                if (input.has_value()) {
                    if (input.value().default_text == "userName")
                        usr_name = input.value().input;
                    else if (input.value().default_text == "Server ip")
                        ip = input.value().input;
                }
            }
            std::cout << ip << " username " << usr_name << std::endl;
            user = usr_name;
            std::string command = "mkdir ../records/" + user + " 2>/dev/null";
            std::string command_2 = "mkdir ../exports/" + user + " 2>/dev/null";
            system(command.c_str());
            system(command_2.c_str());
            my_client = std::make_unique<Client>(io_context, ip, "3001", usr_name);
            for (int i = 0; i < 10; i++)
                my_client->connect();
            std::cout << select_2 << std::endl;
            my_client->authenticate(usr_name, select_2);
            io_context.run_for(std::chrono::milliseconds(20));
            my_client->recevM();
        }

        void ignore_ctrl_c() {
            if (window.isOpen()) {
                std::signal(SIGINT, [](int signum) {});
                std::signal(SIGSEGV, [](int signum) {});
            } else {
            }
        }

        void kill_dead_entities() {
            NovaEngine::Registry& reg = my_game[ROOM];
            NovaEngine::hybrid_array<NovaEngine::weapon_t>& weapons = my_game[ROOM].get_registry<NovaEngine::weapon_t>();
            NovaEngine::hybrid_array<NovaEngine::snackbar_t>& snacks = my_game[ROOM].get_registry<NovaEngine::snackbar_t>();
            for (auto& entity : reg.get_entities()) {
                auto& weapon = weapons[entity];
                if (weapon.has_value() && weapon.value().lives == 0) {
                    reg.kill_entity(entity);
                }
            }
            auto snack = snacks[7];
            auto snack2 = snacks[8];
            auto &pos = reg.get_registry<NovaEngine::position_t>();
            if (snack.has_value() && snack.value().is_expired()) {
                pos[7].reset();
            }
            if (snack2.has_value() && snack2.value().is_expired()) {
                pos[8].reset();
            }
        }

        void process_ui_option(std::string compo) {
            if (clock.getElapsedTime().asSeconds() < 0.2)
                return;
            if (compo == "volume_plus" && volume < 100) {
                auto &rect = my_game[OPTIONS].get_component<NovaEngine::sprite_t>(21);
                volume += 10;
                if (rect.has_value()) {
                    rect.value().size.x = volume / 100.0;
                }
                handler->set_volume(volume);
                clock.restart();
            } else if (compo == "volume_minus" && volume > 0) {
                auto &rect = my_game[OPTIONS].get_component<NovaEngine::sprite_t>(21);
                volume -= 10;
                if (rect.has_value()) {
                    rect.value().size.x = volume / 100.0;
                }
                handler->set_volume(volume);
                clock.restart();
            } else if (compo == "up_key") {
                bind_selector = "up";
                clock.restart();
            } else if (compo == "down_key") {
                bind_selector = "down";
                clock.restart();
            } else if (compo == "left_key") {
                bind_selector = "left";
                clock.restart();
            } else if (compo == "right_key") {
                bind_selector = "right";
                clock.restart();
            } else if (compo == "shoot_key") {
                bind_selector = "shoot";
                clock.restart();
            } else if (compo == "auto_key") {
                bind_selector = "auto";
                clock.restart();
            }
        }

        void proccess_ui(std::string compo, boost::asio::io_context& io_context) {
            if (compo == "save" && clock.getElapsedTime().asSeconds() > 0.5) {
                NovaEngine::sprite_t &spr = my_game[ROOM_INFO].get_component<NovaEngine::sprite_t>(9).value();
                record = !record;
                std::cout << "Recording..." << record << std::endl;
                if (record)
                    spr.asset_name = "save_mns";
                else
                    spr.asset_name = "save_pls";
                clock.restart();
            }
            if (compo == "navigate_left" && clock.getElapsedTime().asSeconds() > 0.5) {
                skin_index++;
                if (skin_index > 5)
                    skin_index = 0;
                NovaEngine::sprite_t &spr = my_game[path].get_component<NovaEngine::sprite_t>(7).value();
                NovaEngine::text_t &name = my_game[path].get_component<NovaEngine::text_t>(8).value();
                spr.asset_name = skins[skin_index];
                name.text = skins[skin_index];
                clock.restart();
            } if (compo == "navigate_right" && clock.getElapsedTime().asSeconds() > 0.5) {
                skin_index--;
                if (skin_index < 0)
                    skin_index = 5;
                NovaEngine::sprite_t &spr = my_game[path].get_component<NovaEngine::sprite_t>(7).value();
                NovaEngine::text_t &name = my_game[path].get_component<NovaEngine::text_t>(8).value();
                spr.asset_name = skins[skin_index];
                name.text = skins[skin_index];
                clock.restart();
            } if (compo == "play" && clock.getElapsedTime().asSeconds() > 0.5) {
                path = LOGIN;
            } if (compo == "leader" && clock.getElapsedTime().asSeconds() > 0.5) {
                path = SCOREBOARD;
                history.push_back(path);
            }
            if (compo == "Quit") {
                path = QUIT;
            } else if (compo == "Survival") {
                path = SURVIVAL;
            } else if (compo == "Option") {
                path = OPTIONS;
            } else if (compo == "Start") {
                path = LOGIN;
            } else if (compo == "Connect") {
                get_and_send(io_context);
                path = LOADING;
            } else if (compo == "back" && clock.getElapsedTime().asSeconds() > 0.5) {
                if (history.back() == HISTORY) {
                    handler->stop_music("intro sound");
                    handler->play_music("theme 1", volume, true);
                }
                history.pop_back();
                if (history.empty())
                    path = MENU;
                else
                    path = history.back();
                // select = 0;
                // select_2 = 0;
                clock.restart();
            }
            if (path == LOADING) {
                my_client->recevM();
                if (my_client->isReadyToPlay()) {
                    handler->stop_music("theme 1");
                    handler->play_music("main sound", volume, true);
                    NovaEngine::hybrid_array<NovaEngine::snackbar_t>& snacks = my_game[ROOM_INFO].get_registry<NovaEngine::snackbar_t>();
                    auto snack = snacks[7];
                    auto snack2 = snacks[8];
                    snack.value().restart();
                    snack2.value().restart();
                    path = ROOM;
                    add_main_player(my_game[path], my_client->getplayerid(), NovaEngine::position_t(my_client->getPosX(), my_client->getPosY()), "BJ-551");
                } else {
                    io_context.run_for(std::chrono::milliseconds(16));
                }
            }
            process_ui_option(compo);
        }

        void update_rooms_world() {
            if (!my_client->isReadyToPlay()) {
                path = GAME_OVER;
                set_many_score(my_game[GAME_OVER], my_client->getAllPlayers());
                nlohmann::json data = {
                    {"name", my_client->get_username()},
                    {"score", my_client->getScore()}
                };
                scores["scoreboard"].push_back(data);
                std::ofstream file("../assets/.save/scoreboard.json");
                if (file.is_open()) {
                    file << scores.dump(4);
                    file.close();
                }
                return;
            }
            NovaEngine::Registry& reg = my_game[ROOM];
            NovaEngine::hybrid_array<NovaEngine::position_t>& positions = reg.get_registry<NovaEngine::position_t>();
            if (!my_client->getIncomingEntities().empty()) {
                for (auto &it : my_client->getIncomingEntities()) {
                    std::stringstream ss(it);
                    std::string type;
                    std::size_t id;
                    int x, y, category;
                    ss >> type;
                    if (type == "Weapon") {
                        ss >> id >> x >> y;
                        if (positions[id].has_value()) {
                            NovaEngine::position_t &pos = positions[id].value();
                            pos._x = x;
                            pos._y = y;
                        } else {
                            other_player_attack(my_game[ROOM], id, NovaEngine::position_t(x, y));
                        }
                    } if (type == "Enemy") {
                        std::string name;
                        ss >> name >> id >> x >> y;
                        if (positions[id].has_value()) {
                            NovaEngine::position_t &pos = positions[id].value();
                            pos._x = x;
                            pos._y = y;
                        } else {
                            if (name == "First") {
                                handler->stop_music("main sound");
                                volume = 100;
                                handler->play_music("first", volume, true);
                            } else if (name == "Second") {
                                handler->stop_music("level 2");
                                volume = 100;
                                handler->play_music("b2", volume, true);
                            } else if (name == "Third") {
                                handler->stop_music("level 3");
                                volume = 100;
                                handler->play_music("b2", volume, true);
                            }
                            add_enemy(my_game[ROOM], id, NovaEngine::position_t(x, y), name);
                        }
                    } if (type == "Killed") {
                        ss >> id;
                        if (positions[id].has_value() && positions[id].value()._x > 0 && positions[id].value()._x < 1900) {
                            NovaEngine::position_t &pos = positions[id].value();
                            add_explosion(my_game[ROOM_INFO], pos);
                            handler->play_audio("boom", volume, false);
                        }
                        reg.kill_entity(id);
                    } if (type == "Bonus") {
                        ss >> id >> x >> y;
                        if (positions[id].has_value()) {
                            NovaEngine::position_t &pos = positions[id].value();
                            pos._x = x;
                            pos._y = y;
                        } else {
                            bonus_pop_out(my_game[ROOM], id, NovaEngine::position_t(x, y));
                        }
                    } if (type == "Attack") {
                        ss >> category >> id >> x >> y;
                        if (positions[id].has_value()) {
                            NovaEngine::position_t &pos = positions[id].value();
                            pos._x = x;
                            pos._y = y;
                        } else {
                            handler->play_audio("shoot", volume, true);
                            ennemy_attack(my_game[ROOM], id, NovaEngine::position_t(x, y), category);
                        }
                    } if (type == "Bonus_shoot") {
                        ss >> category >> id >> x >> y;
                        if (positions[id].has_value()) {
                            NovaEngine::position_t &pos = positions[id].value();
                            pos._x = x;
                            pos._y = y;
                        } else {
                            bonus_attack(my_game[ROOM], id, NovaEngine::position_t(x, y), category);
                        }
                    }
                }
            }
            my_client->clear_entities();
            NovaEngine::text_t &score = my_game[ROOM_INFO].get_component<NovaEngine::text_t>(3).value();
            score.text = "Score: " + std::to_string(my_client->getScore()) + " pts";
            // std::size_t lives = my_client->getLives();
            // if (lives == 2)
            //     reg.kill_entity(6);
            // else if (lives == 1)
            //     reg.kill_entity(5);
            // else if (lives == 0)
            //     reg.kill_entity(4);
            for (auto &it : my_client->getAllPlayers()) {
                std::size_t id = it.first;
                if (id == my_client->getplayerid() && my_client->is_manual())
                    continue;
                if (positions[id].has_value()) {
                    NovaEngine::position_t &pos = positions[id].value();
                    pos._x = it.second.pos_x;
                    pos._y = it.second.pos_y;
                } else {
                    add_player(reg, id, NovaEngine::position_t(it.second.pos_x + 35, it.second.pos_y));
                }
            }
            for (auto &it : my_game[ROOM_INFO].get_message_queue()) {
                std::stringstream ss(it);
                std::string type;
                int ix;
                ss >> type >> ix;
                if (type == "Snack_expires") {
                    my_game[ROOM_INFO].kill_entity(ix);
                }
            }
            my_game[ROOM_INFO].get_message_queue().clear();
        }

        void sendBye() {
            if (my_client.get() != nullptr)
                my_client->sendM("bye");
        }

        void next_level() {
            my_client->reset_level();
            NovaEngine::sprite_t &b1 = my_game[ROOM_INFO].get_component<NovaEngine::sprite_t>(0).value();
            NovaEngine::sprite_t &b2 = my_game[ROOM_INFO].get_component<NovaEngine::sprite_t>(1).value();
            NovaEngine::sprite_t &b3 = my_game[ROOM_INFO].get_component<NovaEngine::sprite_t>(2).value();
            level++;
            if (level == 1) {
                b1.asset_name = "and1";
                b2.asset_name = "and2";
                b3.asset_name = "and3";
                handler->stop_music("first");
                handler->play_music("level 2", volume, true);
            }
            if (level == 2) {
                b1.asset_name = "scorp1";
                b2.asset_name = "scorp2";
                b3.asset_name = "scorp3";
                handler->stop_music("level 2");
                handler->play_music("level 3", volume, true);
            }
            if (level == 3) {
                b1.asset_name = "hol1";
                b2.asset_name = "hol2";
                b3.asset_name = "hol3";
                handler->stop_music("level 3");
                handler->play_music("level 2", volume, true);
            }
            add_snack_bar(my_game[ROOM_INFO], levels[level]);
        }

        void switch_smooth(int x, int y, int num) {
            auto &pos = my_game[MENU].get_component<NovaEngine::position_t>(3);
            auto &s1 = my_game[MENU].get_component<NovaEngine::sprite_t>(x);
            auto &s2 = my_game[MENU].get_component<NovaEngine::sprite_t>(y);
            std::stringstream ss1(s1.value().asset_name), ss2(s2.value().asset_name);
            pos.value()._y = 320 + (100 * num);
            std::string name1, name2;
            ss1 >> name1;
            ss2 >> name2;
            s1.value().size = sf::Vector2f(1, 1);
            s1.value().asset_name = name1;
            s2.value().asset_name = name2 + " hover";
            s2.value().size = sf::Vector2f(1.1, 1);
        }

        void switch_side(int prev, int next, path_t &p) {
            auto &pos = my_game[p].get_component<NovaEngine::position_t>(7);
            auto &spr = my_game[p].get_component<NovaEngine::sprite_t>(prev + 8);
            auto &spr_2 = my_game[p].get_component<NovaEngine::sprite_t>(next + 8);
            std::stringstream ss(spr.value().asset_name), ss_2(spr_2.value().asset_name);
            std::string name, name_2;
            pos.value()._x = 120 + (next * 600);
            ss >> name;
            ss_2 >> name_2;
            spr.value().asset_name = name;
            spr_2.value().asset_name = name_2 + " hover";
        }

        void set_key_name(std::string edit) {
            int entity = 0;
            if (edit == "up")
                entity = 9;
            else if (edit == "down")
                entity = 10;
            else if (edit == "left")
                entity = 11;
            else if (edit == "right")
                entity = 12;
            else if (edit == "shoot")
                entity = 13;
            else if (edit == "auto")
                entity = 14;
            auto &text = my_game[OPTIONS].get_component<NovaEngine::text_t>(entity);
            text.value().text = "click to edit " + edit + " move " + get_key_name(inputBindings[edit]);
        }

        void key_handler() {
            if (sf::Keyboard::isKeyPressed(inputBindings["right"])) {
                if (path == ROOM)
                    my_client->sendM("right");
            } else if (sf::Keyboard::isKeyPressed(inputBindings["left"])) {
                if (path == ROOM)
                    my_client->sendM("left");
            } else if (sf::Keyboard::isKeyPressed(inputBindings["up"])) {
                if (path == ROOM)
                    my_client->sendM("up");
            } else if (sf::Keyboard::isKeyPressed(inputBindings["down"])) {
                if (path == ROOM)
                    my_client->sendM("down");
            } else if (sf::Keyboard::isKeyPressed(inputBindings["auto"]) && clock.getElapsedTime().asSeconds() > 0.5) {
                if (path == ROOM) {
                    my_client->sendM("auto");
                    if (my_game[ROOM].get_component<NovaEngine::controllable_t>(my_client->getplayerid()).has_value()) {
                        my_game[ROOM].get_component<NovaEngine::controllable_t>(my_client->getplayerid()).reset();
                    } else {
                        my_game[ROOM].add_component<NovaEngine::controllable_t>(my_client->getplayerid(), NovaEngine::controllable_t());
                    }
                }
                clock.restart();
            }
            if (sf::Keyboard::isKeyPressed(inputBindings["shoot"]) && !shooting && path == ROOM) {
                handler->play_audio("beam", volume, true);
                shooting = true;
                shoot_clk.restart();
            }
            if (path == MENU && sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                if (path == MENU)
                    handler->play_audio("click sound", volume, false);
                if (select == 0) {
                    switch_smooth(4, 5, 1);
                    select = 1;
                } else if (select == 1) {
                    switch_smooth(5, 6, 2);
                    select = 2;
                } else if (select == 2) {
                    switch_smooth(6, 7, 3);
                    select = 3;
                } else if (select == 3) {
                    switch_smooth(7, 4, 0);
                    select = 0;
                }
            } if (path == MENU && sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                if (path == MENU)
                    handler->play_audio("click sound", volume, false);
                if (select == 0) {
                    switch_smooth(4, 7, 3);
                    select = 3;
                } else if (select == 1) {
                    switch_smooth(5, 4, 0);
                    select = 0;
                } else if (select == 2) {
                    switch_smooth(6, 5, 1);
                    select = 1;
                } else if (select == 3) {
                    switch_smooth(7, 6, 2);
                    select = 2;
                }
            } if ((path == MODE || path == PLAY) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                if (path == MODE || path == PLAY)
                    handler->play_audio("click sound", volume, false);
                if (select_2 == 0 && (path == MODE || path == PLAY)) {
                    switch_side(0, 1, path);
                    select_2 = 1;
                } else if (select_2 == 1 && (path == MODE || path == PLAY)) {
                    switch_side(1, 2, path);
                    select_2 = 2;
                } else if (select_2 == 2 && (path == MODE || path == PLAY)) {
                    switch_side(2, 0, path);
                    select_2 = 0;
                }
            } if ((path == MODE || path == PLAY) &&  sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                if (path == MODE || path == PLAY)
                    handler->play_audio("click sound", volume, false);
                if (select_2 == 0 && (path == MODE || path == PLAY)) {
                    switch_side(0, 2, path);
                    select_2 = 2;
                } else if (select_2 == 1 && (path == MODE || path == PLAY)) {
                    switch_side(1, 0, path);
                    select_2 = 0;
                } else if (select_2 == 2 && (path == MODE || path == PLAY)) {
                    switch_side(2, 1, path);
                    select_2 = 1;
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
                handler->play_audio("click sound", volume, false);
                if (path == MODE) {
                    if (select_2 == 0 && clock.getElapsedTime().asSeconds() > 0.5) {
                        path = PLAY;
                        select_2 = 0;
                        history.push_back(path);
                        clock.restart();
                    } else if (select_2 == 1 && clock.getElapsedTime().asSeconds() > 0.5) {
                        can_play = true;
                        handler->stop_music("theme 1");
                        handler->play_music("main sound", volume, true);
                        clock.restart();
                        game_clk.restart();
                        difficulty = 2;
                        path = SURVIVAL;
                        add_survival_player(my_game[SURVIVAL], 9, NovaEngine::position_t(100, 500), skins[skin_index]);
                        history.push_back(path);
                        clock.restart();
                    } else if (select_2 == 2 && clock.getElapsedTime().asSeconds() > 0.5) {
                        select_2 = 1;
                        path = SPACESHIP;
                        history.push_back(path);
                        clock.restart();
                    }
                } else if (path == MENU) {
                    if (select == 0 && clock.getElapsedTime().asSeconds() > 0.5) {
                        path = MODE;
                        history.push_back(path);
                        clock.restart();
                    } else if (select == 1 && clock.getElapsedTime().asSeconds() > 0.5) {
                        path = OPTIONS;
                        history.push_back(path);
                        clock.restart();
                    } else if (select == 2 && clock.getElapsedTime().asSeconds() > 0.5) {
                        path = HISTORY;
                        handler->stop_music("theme 1");
                        handler->play_music("intro sound", volume, true);
                        history.push_back(path);
                        clock.restart();
                    } else if (select == 3 && clock.getElapsedTime().asSeconds() > 0.5) {
                        path = QUIT;
                        history.push_back(path);
                        clock.restart();
                    }
                } else if (path == PLAY && clock.getElapsedTime().asSeconds() > 0.5) {
                    handler->play_audio("shoot", volume, false);
                    select_2 += 2;
                    path = LOGIN;
                    history.push_back(path);
                    clock.restart();
                }
            }
        }

        void event_handler(sf::Event &event, bool &run_sys) {
            if (event.type == sf::Event::KeyPressed && path == ROOM && my_client->isReadyToPlay()) {
                my_game[path].run_standard_system();
                run_sys = true;
                my_client->setPos(my_game[ROOM].get_component<NovaEngine::position_t>(my_client->getplayerid()).value()._x,
                my_game[ROOM].get_component<NovaEngine::position_t>(my_client->getplayerid()).value()._y);
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (path != ROOM)
                    handler->play_audio("click sound", volume, false);
            }
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (bind_selector != "") {
                    inputBindings[bind_selector] = event.key.code;
                    set_key_name(bind_selector);
                    bind_selector = "";
                }
            } if (event.type == sf::Event::KeyReleased && event.key.code == inputBindings["shoot"]) {
                shooting = false;
                if (path == ROOM && my_client->isReadyToPlay()) {
                    int delta = shoot_clk.getElapsedTime().asMilliseconds();
                    if (delta > 0 && delta < 300)
                        my_client->sendM("shoot");
                    else if (delta > 300 && delta < 800)
                        my_client->sendM("long shoot 0");
                    else if (delta > 800 && delta < 1200)
                        my_client->sendM("long shoot 1");
                    else if (delta > 1200 && delta < 1700)
                        my_client->sendM("long shoot 2");
                    else if (delta > 1700)
                        my_client->sendM("long shoot 3");
                }
                handler->stop_audio("beam");
                if (path == ROOM)
                    handler->play_audio("shoot", volume, false);
            }
            key_handler();
        }

        void add_if_not_registered() {
            if (my_client.get() == nullptr)
                return;
            bool exist = false;
            for (const auto &it : scores) {
                if (it.contains(my_client->get_username())) {
                    exist = true;
                    break;
                }
            }
            if (exist)
                return;
            nlohmann::json data = {
                {"name", my_client->get_username()},
                {"score", my_client->getScore()}
            };
            scores["scoreboard"].push_back(data);
            std::ofstream file("../assets/.save/scoreboard.json");
            if (file.is_open()) {
                file << scores.dump(4);
                file.close();
            }
        }

        void run() {
            std::string compo, compo_2;
            boost::asio::io_context io_context;
            bool run_sys = false;
            window.setKeyRepeatEnabled(true);
            handler->play_music("theme 1", volume, true);
            std::thread saver_thread(saveFrames);
            while (window.isOpen()) {
                sf::Event event;
                run_sys = false;
                ignore_ctrl_c();
                while (window.pollEvent(event)) {
                    my_game[path].run_event_system(event, window, *handler);
                    event_handler(event, run_sys);
                }
                if (path == QUIT) {
                    //sendBye();
                    window.close();
                }
                window.clear();
                if (path != QUIT && start) {
                    if (!run_sys)
                        my_game[path].run_standard_system();
                    if (path == ROOM) {
                        my_game[ROOM_INFO].run_rendering_system(window, *handler);
                        my_game[ROOM_INFO].run_standard_system();
                        compo_2 = my_game[ROOM_INFO].run_ui_system(window, event);
                    }
                    my_game[path].run_rendering_system(window,*handler);
                    compo = my_game[path].run_ui_system(window, event);
                }
                if (path != HISTORY && path != LOGIN && path != LOADING) {
                    if (path == ROOM)
                        manage_paralax(my_game[ROOM_INFO]);
                    else
                        manage_paralax(my_game[path]);
                }
                if (!start)
                   handler->draw_asset(window, "nova", sf::Vector2f(1, 1), sf::Vector2f(650, 320));
                if (!start && clock.getElapsedTime().asSeconds() > 4.45) {
                    start = true;
                    clock.restart();
                }
                window.display();
                if (path == ROOM && start) {
                    update_rooms_world();
                    kill_dead_entities();
                    io_context.run_for(std::chrono::milliseconds(16));
                }
                if (path == ROOM && my_client->level_changed()) {
                    next_level();
                } if (path == SURVIVAL && can_play) {
                    handle_attack_request(my_game[SURVIVAL]);
                    if (game_clk.getElapsedTime().asSeconds() > 4) {
                        spawn_ennemy(my_game[SURVIVAL], "boul", difficulty);
                        game_clk.restart();
                    }
                    if (clock.getElapsedTime().asSeconds() > 10) {
                        level++;
                        clock.restart();
                    }
                }
                if (record && path == ROOM && game_clk.getElapsedTime().asSeconds() >= (1 / 20) && frameQueue.size() < 300) {
                    sf::Image screenshot = window.capture();
                    std::lock_guard<std::mutex> lock(queueMutex);
                    frameQueue.push(screenshot);
                    frame_condition.notify_one();
                    game_clk.restart();
                }
                proccess_ui(compo, io_context);
                if (path == ROOM)
                    proccess_ui(compo_2, io_context);
            }
            stop_thread = true;
            handler->set_volume(0);
            frame_condition.notify_one();
            saver_thread.join();
            add_if_not_registered();
            exit(0);
        }

    private:
        std::unique_ptr<Client> my_client;
        sf::RenderWindow window;
        std::unique_ptr<NovaEngine::Asset_handler> handler;
        path_t path;
        int level;
        int select;
        int select_2;
        std::string bind_selector;
        int volume;
        bool record;
        std::vector<std::string> levels;
        sf::Clock clock;
        sf::Clock game_clk;
        int difficulty;
        bool start;
        bool can_play;
        std::string weapons;
        std::vector<std::string> skins;
        int skin_index;
        std::vector<path_t> history;
        std::map<std::string, sf::Keyboard::Key> inputBindings;
        nlohmann::json scores;
        std::vector<NovaEngine::Registry> my_game;
        sf::Clock shoot_clk;
        bool shooting = false;
};
