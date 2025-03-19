#include "../../include/network/server/serveur.hpp"
#include "../../../nova_engine/include/core/system.hpp"
#include "../../../nova_engine/include/core/register.hpp"

bool UdpServer::connect_username(Message _msg)
{
    if (_msg.header == 0x01) {
        Message new_msg;
        std::cout << "Player [" << client_endpoint << "] connected..." << std::endl;
        new_msg.header = 0x02;
        new_msg.pos_x = 0;
        new_msg.pos_y = 0;
        std::string msg = binary_protocol.crypt_message(new_msg);
        std::string compress_msg = compress_data.compressData(msg);
        server_socket.async_send_to(boost::asio::buffer(compress_msg), client_endpoint,
        std::bind(&UdpServer::handle_send, this, std::placeholders::_1, "Ask username", client_endpoint));
        return true;
    }
    return false;
}

bool UdpServer::username_position(Message _msg)
{
    if (_msg.header == 0x02) {
        Message new_msg;
        if (client_endpoint_list.size() == 0) {
            nbr_players_game = _msg.pos_x;
        }
        client_endpoint_list.push_back(client_endpoint);
        new_msg.header = 0x03;
        new_msg.id = network_register.spawn_entity();
        std::cout << "Create new player with Id: {" << static_cast<int>(new_msg.id)  << "}" << std::endl;
        client_id_list.push_back(new_msg.id);
        new_msg.pos_x = 40;
        new_msg.pos_y = increment_pos_y;
    
        network_register.add_component<NovaEngine::position_t>(new_msg.id, NovaEngine::position_t(0, new_msg.pos_y));
        network_register.add_component<NovaEngine::velocity_t>(new_msg.id, NovaEngine::velocity_t(0, 0, 0, 0));
        // network_register.add_component<NovaEngine::acceleration_t>(new_msg.id, NovaEngine::acceleration_t(0, 0));
        network_register.add_component<NovaEngine::actor_t>(new_msg.id, NovaEngine::actor_t(3, "Player"));
        network_register.add_component<NovaEngine::score_t>(new_msg.id, NovaEngine::score_t(0));
        network_register.add_component<NovaEngine::rigid_body_t>(new_msg.id, NovaEngine::rigid_body_t(true));
        network_register.add_component<NovaEngine::box_collider_t>(new_msg.id, NovaEngine::box_collider_t(66, 66));

        increment_pos_y += 100;
        std::string msg = binary_protocol.crypt_message(new_msg);
        std::string compress_msg = compress_data.compressData(msg);
        server_socket.async_send_to(boost::asio::buffer(compress_msg), client_endpoint,
        std::bind(&UdpServer::handle_send, this, std::placeholders::_1, "Position & ID", client_endpoint));
        return true;
    }
    return false;
}
