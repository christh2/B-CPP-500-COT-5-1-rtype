#include "../../include/network/server/serveur.hpp"
#include "../../../nova_engine/include/core/system.hpp"
#include "../../../nova_engine/include/core/register.hpp"

void UdpServer::notify_death(std::size_t id_killed, std::size_t id_killer, std::size_t score)
{
    Message new_msg;
    new_msg.header = 0x07;
    new_msg.pos_x = id_killer;
    new_msg.pos_y = score;
    new_msg.id = id_killed;
    std::string msg = binary_protocol.crypt_message(new_msg);
    std::string compress_msg = compress_data.compressData(msg);
    //std::cout << "Entity n° " << id_killed << " is dead" << std::endl;
    for (auto client : client_endpoint_list) {
        std::string txt =  "Entity " + std::to_string(id_killed) + " Killed by entity " + std::to_string(id_killer);
        server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
        std::bind(&UdpServer::handle_send, this, std::placeholders::_1, txt, client));
    }
}

void UdpServer::check_dead_actor() {
    NovaEngine::hybrid_array<NovaEngine::actor_t>& actors = network_register.get_registry<NovaEngine::actor_t>();
    NovaEngine::hybrid_array<NovaEngine::weapon_t>& weapons = network_register.get_registry<NovaEngine::weapon_t>();
    NovaEngine::hybrid_array<NovaEngine::rigid_body_t>& bodies = network_register.get_registry<NovaEngine::rigid_body_t>();
    NovaEngine::hybrid_array<NovaEngine::score_t>& scores = network_register.get_registry<NovaEngine::score_t>();
    NovaEngine::hybrid_array<NovaEngine::linked_t>& links = network_register.get_registry<NovaEngine::linked_t>();
    for (auto& entity : network_register.get_entities()) {
        auto& actor = actors[entity];
        auto& body = bodies[entity];
        auto& score = scores[entity];
        int killer = -1;
        if (actor.has_value() && actor.value().live <= 0 && body.has_value()) {
            if (body.value().id != -1 && weapons[body.value().id].has_value()) {
                auto &weapon = weapons[body.value().id];
                if (actors[weapon.value().id].has_value()) {
                    killer = weapon.value().id;
                    score = scores[weapon.value().id];
                }
                for (auto i : network_register.get_entities()) {
                    if (links[i].has_value())
                        links[i].value().remove_entity(entity);
                }
            }
            try {
                notify_death(entity, killer, score.has_value() ? score.value() : 0);
                if (actor.value().name == "Crevette") {
                    //if (_next_level == false) {
                        next_level();
                        //_next_level = true;
                    //}
                    std::cout << "\nCrevette Boss is killed\n\n";
                }
                if (actor.value().name == "Second boss") {
                    //if (_next_level == false) {
                        next_level();
                        //_next_level = true;
                    //}
                    std::cout << "\nSecond Boss is killed\n\n";
                }
                if (actor.value().name == "Third boss") {
                    //if (_next_level == false) {
                        next_level();
                        //_next_level = true;
                    //}
                    std::cout << "\nThird Boss is killed\n\n";
                }
                if (actor.value().name == "Fourth boss") {
                    //if (_next_level == false) {
                        next_level();
                        //_next_level = true;
                    //}
                    std::cout << "\nFourth Boss is killed\n\n";
                }
                if (actor.value().name == "Player") {
                    nbr_players--;
                    if (nbr_players == 0) {
                        game_over();
                    }
                }
            } catch (const std::exception &e) {
                // Gérer les exceptions standard
                std::cerr << "Exception caught in standard system: " << e.what() << std::endl;
            } catch (...) {
                // Gérer les exceptions inconnues
                std::cerr << "Unknown exception caught in standard system." << std::endl;
            }
            
            network_register.kill_entity(entity);
        }
    }
    for (auto& entity : network_register.get_entities()) {
        auto& weapon = weapons[entity];
        if (weapon.has_value() && weapon.value().lives <= 0) {
            notify_death(entity, entity, 0);
            network_register.kill_entity(entity);
        }
    }
}
