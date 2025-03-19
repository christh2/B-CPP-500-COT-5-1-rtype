#include "../../include/network/server/serveur.hpp"
#include "demo_network/demo_network.hpp"

UdpServer::UdpServer(boost::asio::io_context& io_context) 
: server_socket(io_context, boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string("0.0.0.0"), 3001)),
signal_c(io_context, SIGINT),
io_context(io_context),
run_pos_timer(io_context, boost::asio::chrono::milliseconds(25)),
run_sys_timer(io_context, boost::asio::chrono::milliseconds(25)),
_timer_papillon(io_context, boost::asio::chrono::seconds(2)),
_timer_battle_ship(io_context, boost::asio::chrono::seconds(5)),
_timer_commandant(io_context, boost::asio::chrono::seconds(7)),
_timer_alien(io_context, boost::asio::chrono::seconds(3)),
_timer_pique(io_context, boost::asio::chrono::seconds(4)),
_timer_robot(io_context, boost::asio::chrono::seconds(6)),
//_timer_first_boss(io_context, boost::asio::chrono::seconds(38)),
_timer_bonus(io_context, boost::asio::chrono::seconds(12))
{
    signal_c.async_wait(std::bind(&UdpServer::handle_signal, this));
    NovaEngine::init_nova_component(network_register);
    NovaEngine::init_nova_system(network_register);
    std::cout << "Welcome to Nova...\n" << std::endl;
    timer_run_sys();
    receive_client_msg();
}

void UdpServer::receive_client_msg()
{
    rev_buff.fill(0);
    //network_register.run_standard_system();
    send_start_game();
    timer_enemy();
    check_dead_actor();
    server_socket.async_receive_from(
        boost::asio::buffer(rev_buff), client_endpoint,
        std::bind(&UdpServer::handle_receive, this,
        std::placeholders::_1, std::placeholders::_2)
    );
}

bool UdpServer::get_ai_request(Message _msg)
{
    NovaEngine::hybrid_array<NovaEngine::actor_t>& actors = network_register.get_registry<NovaEngine::actor_t>();
    NovaEngine::hybrid_array<NovaEngine::weapon_t>& weapons = network_register.get_registry<NovaEngine::weapon_t>();
    NovaEngine::hybrid_array<NovaEngine::behavior_t>& behaves = network_register.get_registry<NovaEngine::behavior_t>();
    NovaEngine::hybrid_array<NovaEngine::linked_t>& linked = network_register.get_registry<NovaEngine::linked_t>();
    std::vector<std::size_t> links = {};
    for (auto &it : network_register.get_entities()) {
        auto &actor = actors[it];
        auto &weapon = weapons[it];
        if (actor.has_value() && actor.value().name != "Player") {
            links.push_back(it);
        }
        if (weapon.has_value() && weapon.value().name != "bonus") {
            links.push_back(it);
        }
    }
    if (_msg.header == 0x08A) {
        std::cout << "AI request received..." << std::endl;
        network_register.add_component<NovaEngine::behavior_t>(_msg.id, NovaEngine::behavior_t("../assets/scripts/player_ai.lua", 500));
        std::cout << "Got " << links.size() << " links" << std::endl;
        network_register.add_component<NovaEngine::linked_t>(_msg.id, NovaEngine::linked_t(links, false));
        return true;
    } else if (_msg.header == 0x08B) {
        std::cout << "Manual request received..." << std::endl;
        if (network_register.get_component<NovaEngine::behavior_t>(_msg.id).has_value()) {
            network_register.get_component<NovaEngine::behavior_t>(_msg.id).reset();
        }
        return true;
    }
    for (int i = 0; i < nbr_players; i++) {
        auto &behave = behaves[i];
        auto &link = linked[i];
        if (behave.has_value() && link.has_value()) {
            link.value().links = {};
            link.value().links = links;
        }
    }
    return false;
}

void UdpServer::handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred)
{
    if (!error) {
        //std::cout << "Bytes reçus: " << bytes_transferred << std::endl;
        std::string take_msg (rev_buff.data(), bytes_transferred);
        std::string decompress_msg = compress_data.decompressData(take_msg);
        Message _msg = binary_protocol.decrypt_message(decompress_msg);
        //std::cout << "take_msg: " << _msg.header << std::endl;
        
        if (connect_username(_msg)) {
        } else if (username_position(_msg)) {
        } else if (move(_msg)) {
        } else if  (player.shoot(_msg, client_endpoint, network_register, client_endpoint_list, server_socket)) {
        } else if (deconnexion_player(_msg)) {
        } else if (send_restart_game(_msg)) {
        } else if (get_ai_request(_msg)) {
        } else
            error_manager(_msg);
    } else {
        std::cerr << "Erreur de réception : " << error.message() << std::endl;
    }
    receive_client_msg();
}

void UdpServer::timer_run_sys()
{
    //std::cout << "\033[1;32mTimer run sys start\033[0m" << std::endl;
    network_register.run_standard_system();
    run_sys_timer.expires_after(boost::asio::chrono::milliseconds(25));
    run_sys_timer.async_wait(boost::bind(&UdpServer::timer_run_sys, this));

}

void UdpServer::timer_pos_sys()
{
    NovaEngine::hybrid_array<NovaEngine::actor_t>& actors = network_register.get_registry<NovaEngine::actor_t>();
    NovaEngine::hybrid_array<NovaEngine::position_t>& positions = network_register.get_registry<NovaEngine::position_t>();
    NovaEngine::hybrid_array<NovaEngine::velocity_t>& velocities = network_register.get_registry<NovaEngine::velocity_t>();
    for (auto& entity : network_register.get_entities()) {
        auto& actor = actors[entity];
        auto& pos = positions[entity]; 
        if (actor.has_value() && pos.has_value() && actor.value().name == "Battle Ship") {
            NovaEngine::position_t pos_en = pos.value();
            Message new_msg;
            new_msg.id = entity;
            new_msg.header = 0x09A;
            new_msg.pos_x = pos_en._x;
            new_msg.pos_y =  pos_en._y;
            std::string msg = binary_protocol.crypt_message(new_msg);
            std::string compress_msg = compress_data.compressData(msg);
            std::string txt = "Enemy: Battle Ship at {" + std::to_string(new_msg.pos_x) + "} " + std::to_string(new_msg.pos_y);
            for (auto client : client_endpoint_list) {
                server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
                std::bind(&UdpServer::handle_send, this, std::placeholders::_1, txt, client));
            }
        }
        if (actor.has_value() && pos.has_value() && actor.value().name == "Pique") {
            NovaEngine::position_t pos_en = pos.value();
            Message new_msg;
            new_msg.id = entity;
            new_msg.header = 0x091B;
            new_msg.pos_x = pos_en._x;
            new_msg.pos_y =  pos_en._y;
            std::string msg = binary_protocol.crypt_message(new_msg);
            std::string compress_msg = compress_data.compressData(msg);
            std::string txt = "Enemy: Pique at {" + std::to_string(new_msg.pos_x) + "} " + std::to_string(new_msg.pos_y);
            for (auto client : client_endpoint_list) {
                server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
                std::bind(&UdpServer::handle_send, this, std::placeholders::_1, txt, client));
            }
        }
        if (actor.has_value() && pos.has_value() && actor.value().name == "Commandant") {
            NovaEngine::position_t pos_en = pos.value();
            Message new_msg;
            new_msg.header = 0x09C;
            new_msg.id = entity;
            new_msg.pos_x = pos_en._x;
            new_msg.pos_y =  pos_en._y;
            std::string msg = binary_protocol.crypt_message(new_msg);
            std::string compress_msg = compress_data.compressData(msg);
            std::string txt = "Enemy: Commandant at {" + std::to_string(new_msg.pos_x) + "} " + std::to_string(new_msg.pos_y);
            for (auto client : client_endpoint_list) {
                server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
                std::bind(&UdpServer::handle_send, this, std::placeholders::_1, txt, client));
            }
        }
        if (actor.has_value() && pos.has_value() && actor.value().name == "Crevette") {
            NovaEngine::position_t pos_en = pos.value();
            Message new_msg;
            new_msg.id = entity;
            new_msg.header = 0x09F;
            new_msg.pos_x = pos_en._x;
            new_msg.pos_y =  pos_en._y;
            std::string msg = binary_protocol.crypt_message(new_msg);
            std::string compress_msg = compress_data.compressData(msg);
            std::string txt = "Enemy: Crevette at {" + std::to_string(new_msg.pos_x) + "} " + std::to_string(new_msg.pos_y);
            for (auto client : client_endpoint_list) {
                server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
                std::bind(&UdpServer::handle_send, this, std::placeholders::_1, txt, client));
            }
        }
        if (actor.has_value() && pos.has_value() && actor.value().name == "Papillon") {
            NovaEngine::position_t pos_en = pos.value();
            Message new_msg;
            new_msg.id = entity;
            new_msg.header = 0x09B;
            new_msg.pos_x = pos_en._x;
            new_msg.pos_y =  pos_en._y;
            std::string msg = binary_protocol.crypt_message(new_msg);
            std::string compress_msg = compress_data.compressData(msg);
            std::string txt = "Enemy: Papillon at {" + std::to_string(new_msg.pos_x) + "} " + std::to_string(new_msg.pos_y);
            for (auto client : client_endpoint_list) {
                server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
                std::bind(&UdpServer::handle_send, this, std::placeholders::_1, txt, client));
            }
        }
        if (actor.has_value() && pos.has_value() && actor.value().name == "Alien") {
            NovaEngine::position_t pos_en = pos.value();
            Message new_msg;
            new_msg.id = entity;
            new_msg.header = 0x091A;
            new_msg.pos_x = pos_en._x;
            new_msg.pos_y =  pos_en._y;
            std::string msg = binary_protocol.crypt_message(new_msg);
            std::string compress_msg = compress_data.compressData(msg);
            std::string txt = "Enemy: Alien at {" + std::to_string(new_msg.pos_x) + "} " + std::to_string(new_msg.pos_y);
            for (auto client : client_endpoint_list) {
                server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
                std::bind(&UdpServer::handle_send, this, std::placeholders::_1, txt, client));
            }
        }
        if (actor.has_value() && pos.has_value() && actor.value().name == "Robot") {
            NovaEngine::position_t pos_en = pos.value();
            Message new_msg;
            new_msg.id = entity;
            new_msg.header = 0x091C;
            new_msg.pos_x = pos_en._x;
            new_msg.pos_y =  pos_en._y;
            std::string msg = binary_protocol.crypt_message(new_msg);
            std::string compress_msg = compress_data.compressData(msg);
            std::string txt = "Enemy: Robot at {" + std::to_string(new_msg.pos_x) + "} " + std::to_string(new_msg.pos_y);
            for (auto client : client_endpoint_list) {
                server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
                std::bind(&UdpServer::handle_send, this, std::placeholders::_1, txt, client));
            }
        }
        if (actor.has_value() && pos.has_value() && actor.value().name == "Second boss") {
            NovaEngine::position_t pos_en = pos.value();
            Message new_msg;
            new_msg.id = entity;
            new_msg.header = 0x092F;
            new_msg.pos_x = pos_en._x;
            new_msg.pos_y =  pos_en._y;
            std::string msg = binary_protocol.crypt_message(new_msg);
            std::string compress_msg = compress_data.compressData(msg);
            std::string txt = "Enemy: Second at {" + std::to_string(new_msg.pos_x) + "} " + std::to_string(new_msg.pos_y);
            for (auto client : client_endpoint_list) {
                server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
                std::bind(&UdpServer::handle_send, this, std::placeholders::_1, txt, client));
            }
        }
        if (actor.has_value() && pos.has_value() && actor.value().name == "Third boss") {
            NovaEngine::position_t pos_en = pos.value();
            Message new_msg;
            new_msg.id = entity;
            new_msg.header = 0x092F;
            new_msg.pos_x = pos_en._x;
            new_msg.pos_y =  pos_en._y;
            std::string msg = binary_protocol.crypt_message(new_msg);
            std::string compress_msg = compress_data.compressData(msg);
            std::string txt = "Enemy: Second at {" + std::to_string(new_msg.pos_x) + "} " + std::to_string(new_msg.pos_y);
            for (auto client : client_endpoint_list) {
                server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
                std::bind(&UdpServer::handle_send, this, std::placeholders::_1, txt, client));
            }
        }
        if (actor.has_value() && pos.has_value() && actor.value().name == "Fourth boss") {
            NovaEngine::position_t pos_en = pos.value();
            Message new_msg;
            new_msg.id = entity;
            new_msg.header = 0x092F;
            new_msg.pos_x = pos_en._x;
            new_msg.pos_y =  pos_en._y;
            std::string msg = binary_protocol.crypt_message(new_msg);
            std::string compress_msg = compress_data.compressData(msg);
            std::string txt = "Enemy: Second at {" + std::to_string(new_msg.pos_x) + "} " + std::to_string(new_msg.pos_y);
            for (auto client : client_endpoint_list) {
                server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
                std::bind(&UdpServer::handle_send, this, std::placeholders::_1, txt, client));
            }
        }
    }
    NovaEngine::hybrid_array<NovaEngine::weapon_t>& weapons = network_register.get_registry<NovaEngine::weapon_t>();
    for (auto& entity : network_register.get_entities()) {
        auto& weapon = weapons[entity];
        auto& pos = positions[entity];
        if (!weapon.has_value())
            continue;
        auto& actor = actors[weapon.value().id];
        if (weapon.has_value() && weapon.value().lives > 0 && pos.has_value()) {
            NovaEngine::position_t pos_en = pos.value();
            Message new_msg;
            new_msg.id = entity;
            if (actor.has_value() && (weapon.value().name == "enem_weapon" || weapon.value().name == "target_weapon")) {
                new_msg.header = 0x099;
            } else if (actor.has_value() && weapon.value().name == "long_Shoot") {
                new_msg.header = 0x06B;
            } else if (actor.has_value() && weapon.value().name == "basic_weapon") {
                new_msg.header = 0x06;
            } else if (actor.has_value() && weapon.value().name == "bonus_weapon_1") {
                new_msg.header = 0x061A;
            } else if (actor.has_value() && weapon.value().name == "bonus_weapon_2") {
                new_msg.header = 0x062A;
            } else if (actor.has_value() && weapon.value().name == "bonus_weapon_3") {
                new_msg.header = 0x063A;
            } else if (actor.has_value() && weapon.value().name == "bonus_weapon_4") {
                new_msg.header = 0x064A;
            } else if (actor.has_value() && weapon.value().name == "long_Shoot_1") {
                new_msg.header = 0x061B;
            } else if (actor.has_value() && weapon.value().name == "long_Shoot_2") {
                new_msg.header = 0x062B;
            } else if (actor.has_value() && weapon.value().name == "long_Shoot_3") {
                new_msg.header = 0x063B;
            } else if (actor.has_value() && actor.value().level == 1) {
                new_msg.header = 0x06A;
            } else
                new_msg.header = 0x019A;
            new_msg.pos_x = pos_en._x;
            new_msg.pos_y =  pos_en._y;
            std::string msg = binary_protocol.crypt_message(new_msg);
            std::string compress_msg = compress_data.compressData(msg);
            for (auto client : client_endpoint_list) {
                server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
                std::bind(&UdpServer::handle_send, this, std::placeholders::_1, "Shoot", client));
            }
        }
    }
    for (auto &it : network_register.get_message_queue()) {
        if (nbr_players == 0)
            continue;
        std::stringstream ss(it);
        std::string sys, order;
        std::getline(ss, sys, ':');
        std::getline(ss, order, ':');
        std::stringstream ss_order(order);
        std::string type, value, script;
        int shooter, offx, offy;
        ss_order >> type;
        if (type == "shoot") {
            ss_order >> value >> offx >> offy >> script >> shooter;
            std::size_t id = network_register.spawn_entity();
            if (value != "target_weapon") {
                network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(offx, offy));
                network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(0, 0, 0, 0));
                network_register.add_component<NovaEngine::behavior_t>(id, NovaEngine::behavior_t(script, 0.064));
                network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
                network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(53, 12));
                network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
                network_register.add_component<NovaEngine::weapon_t>(id, NovaEngine::weapon_t(shooter, 1, offx, 500, value));
                network_register.add_entity(id);
            } else if (value == "target_weapon") {
                network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(offx, offy));
                network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(0, 0, 0, 0));
                network_register.add_component<NovaEngine::behavior_t>(id, NovaEngine::behavior_t(script, 0.064));
                std::vector<std::size_t> links = {};
                for (int x = 0; x < nbr_players; x++)
                    links.push_back(x);
                network_register.add_component<NovaEngine::linked_t>(id, NovaEngine::linked_t(links));
                NovaEngine::linked_t &link = network_register.get_component<NovaEngine::linked_t>(id).value();
                for (int x = 0; x < nbr_players; x++) {
                    std::size_t id = link.links[x];
                    if (positions[id].has_value() && velocities[id].has_value()) {
                        link.friend_pos[id] = positions[id].value();
                        link.friend_vel[id] = velocities[id].value();
                    }
                }
                network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
                network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(53, 12));
                network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
                network_register.add_component<NovaEngine::weapon_t>(id, NovaEngine::weapon_t(shooter, 1, offx, 500, value));
                network_register.add_entity(id);
            }
        } else if (type == "get_bonus") {
            ss_order >> offx;
            auto &actor = actors[offx];
            if (actor.has_value()) {
                actor.value().level = 1;
            }
            send_bonus_taken(offx);
        } else if (type == "child") {
            ss_order >> value >> offx >> offy >> script >> shooter;
            std::size_t id = network_register.spawn_entity();
            network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(offx, offy));
            network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(1.5, 0, -1, 0));
            network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
            network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(260, 144));
            network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
            network_register.add_component<NovaEngine::actor_t>(id, NovaEngine::actor_t(1, value));
            network_register.add_component<NovaEngine::behavior_t>(id, NovaEngine::behavior_t(script, 0.064));
            network_register.add_component<NovaEngine::score_t>(id, NovaEngine::score_t(40));
            network_register.add_entity(id);
        }
    }
    network_register.get_message_queue().clear();
    run_pos_timer.expires_after(boost::asio::chrono::milliseconds(25));
    run_pos_timer.async_wait(boost::bind(&UdpServer::timer_pos_sys, this));
}


void UdpServer::handle_send(const boost::system::error_code& error, std::string txt,
boost::asio::ip::udp::endpoint client_endpoint)
{
    // if (!error) {
    //     std::cout<< "Send [" << txt  << "] to [" << client_endpoint << "] : Success" << std::endl;
        
    // } else {
    //     std::cout<< "Send [" << txt  << "] to [" << client_endpoint << "] : Failled" << std::endl;
    // }
}

void UdpServer::handle_signal()
{
    deconnexion_server();
    server_socket.close();
    io_context.stop();
    std::exit(0);
}
