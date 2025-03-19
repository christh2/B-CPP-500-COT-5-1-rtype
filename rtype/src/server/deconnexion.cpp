#include "../../include/network/server/serveur.hpp"

bool UdpServer::deconnexion_player(Message _msg)
{
    if (_msg.header == 0x010) {
        Message new_msg;
        new_msg.header = 0x010;
        new_msg.id = _msg.id;
        std::string msg = binary_protocol.crypt_message(new_msg);
        std::string compress_msg = compress_data.compressData(msg);
        std::cout << "Player {" << _msg.id << "} is deconnected\n";
        std::string txt = "Deconnexion of Player {" + std::to_string(new_msg.id) + "}";
        for (auto client : client_endpoint_list) {
            server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
            std::bind(&UdpServer::handle_send, this, std::placeholders::_1, txt, client_endpoint));
        }
        return true;
    }
    return false;
}

bool UdpServer::deconnexion_server()
{
    Message new_msg;
    new_msg.header = 0x011;
    std::string msg = binary_protocol.crypt_message(new_msg);
    std::string compress_msg = compress_data.compressData(msg);
    std::cout << "\nServer " << "shutdown...\n";
    for (auto client : client_endpoint_list) {
        server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
        std::bind(&UdpServer::handle_send, this, std::placeholders::_1, "Server shutdown", client_endpoint));
    }
    return true;
}

void UdpServer::game_over()
{
    Message new_msg;
    new_msg.header = 0x015;
    std::string msg = binary_protocol.crypt_message(new_msg);
    std::string compress_msg = compress_data.compressData(msg);
    for (auto client : client_endpoint_list) {
        server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
        std::bind(&UdpServer::handle_send, this, std::placeholders::_1, "Game Over", client_endpoint));
    }
    std::cout << "\nGame Over...\n";
    std::cout << "\nRestart the game please...\n";
    start = false;
    start_timer = false;
    level = 1;
    nbr_bonus = 0;
    enemy.nbr_alien = 0;
    enemy.nbr_papillon = 0;
    enemy.nbr_commandant = 0;
    enemy.nbr_battle_ship = 0;
    enemy.nbr_robot = 0;
    enemy.nbr_pique = 0;
    enemy.nbr_first_boss = 0;
    enemy.nbr_second_boss = 0;
    enemy.nbr_third_boss = 0;
    enemy.nbr_fourth_boss = 0;

    //handle_signal();
}