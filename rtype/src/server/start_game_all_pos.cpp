#include "../../include/network/server/serveur.hpp"
#include "../../../nova_engine/include/core/system.hpp"
#include "../../../nova_engine/include/core/register.hpp"

void UdpServer::send_start_game()
{
    if (client_endpoint_list.size() == nbr_players_game && start == false) {
        Message new_msg;
        new_msg.header = 0x04;
        std::string msg = binary_protocol.crypt_message(new_msg);
        std::string compress_msg = compress_data.compressData(msg);
        for (auto client : client_endpoint_list) {
            std::cout << "Player ========" << client << std::endl;
            for (int a = 5; a >= 0; a--) {
                std::cout << "Message START to " << client << "  Nbr  " << a << std::endl;
                server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
                std::bind(&UdpServer::handle_send, this, std::placeholders::_1, "Start", client));
            }
        }
        send_position_of_all_client();
        start = true;
        start_timer = true;
        nbr_players = client_endpoint_list.size();
    }
}

bool UdpServer::send_restart_game(Message _msg)
{
    if (_msg.header == 0x016) {
        Message new_msg;
        new_msg.header = 0x04;
        std::string msg = binary_protocol.crypt_message(new_msg);
        std::string compress_msg = compress_data.compressData(msg);
        for (auto client : client_endpoint_list) {
            for (int a = 5; a >= 0; a--) {
                server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
                std::bind(&UdpServer::handle_send, this, std::placeholders::_1, "Start", client));
            }
        }
        send_position_of_all_client();
        start = true;
        start_timer = true;
        return true;
    }
    return false;
}

void UdpServer::send_position_of_all_client()
{
    if (client_id_list.size() > 1) {
        for (auto client : client_endpoint_list) {
            for (auto i : client_id_list) {
                NovaEngine::position_t &pos = network_register.get_component<NovaEngine::position_t>(i).value();
                std::cout << "Player [" << i << "] position: (" << pos._x << ", " << pos._y << ")" << std::endl;
                Message new_msg;
                new_msg.header = 0x051;
                new_msg.id = i;
                new_msg.pos_x = pos._x;
                new_msg.pos_y = pos._y;
                std::string msg = binary_protocol.crypt_message(new_msg);
                std::string compress_msg = compress_data.compressData(msg);
                server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
                    std::bind(&UdpServer::handle_send, this, std::placeholders::_1, "Send all position", client));
            }
        }
    }
}

void UdpServer::send_bonus()
{
    Message new_msg;
    new_msg.header = 0x019A;
    std::size_t id = network_register.spawn_entity();
    new_msg.id = id;
    new_msg.pos_x = 1905;
    new_msg.pos_y = rand() % 1001;
    network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(new_msg.pos_x, new_msg.pos_y));
    network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(1.5, 0, -1, 0));
    network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
    network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(32, 32));
    network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
    network_register.add_component<NovaEngine::weapon_t>(id, NovaEngine::weapon_t(id, 1, new_msg.pos_x, 1900, "bonus"));
    network_register.add_component<NovaEngine::score_t>(id, NovaEngine::score_t(2));
    network_register.add_entity(id);
    std::string msg = binary_protocol.crypt_message(new_msg);
    std::string compress_msg = compress_data.compressData(msg);
    for (auto client : client_endpoint_list) {
        server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
        std::bind(&UdpServer::handle_send, this, std::placeholders::_1, "Bonus", client));
    }
    std::cout << "\nBonus sent\n";
    nbr_bonus--;
}

void UdpServer::send_bonus_taken(std::size_t id)
{
    Message new_msg;
    new_msg.header = 0x019B;
    new_msg.id = id;
    new_msg.pos_x = 0;
    new_msg.pos_y = 0;
    std::string msg = binary_protocol.crypt_message(new_msg);
    std::string compress_msg = compress_data.compressData(msg);
    for (auto client : client_endpoint_list) {
        server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
        std::bind(&UdpServer::handle_send, this, std::placeholders::_1, "Bonus", client));
    }
    std::cout << "\nBonus taken\n";
}

void UdpServer::timer_bonus()
{
    if (nbr_bonus > 0) {
        send_bonus();
        _timer_bonus.expires_after(boost::asio::chrono::seconds(12));
        _timer_bonus.async_wait(boost::bind(&UdpServer::timer_bonus, this));
    }
}
