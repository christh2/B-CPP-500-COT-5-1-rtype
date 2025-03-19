#include "../../include/network/server/serveur.hpp"
#include "../../../nova_engine/include/core/system.hpp"
#include "../../../nova_engine/include/core/register.hpp"

bool Player::shoot(Message _msg, boost::asio::ip::udp::endpoint client_endpoint,
NovaEngine::Registry &network_register, std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
boost::asio::ip::udp::socket &server_socket)
{
    if (_msg.header == 0x06) {
        //std::cout << "Player [" << client_endpoint << "] with Id: {" << _msg.id << "} shoot"  << std::endl;
        
        //crée une entité avec une vitesse constante pour le faire avancer, et weapon comme comportement
        std::size_t id = network_register.spawn_entity();
        network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(_msg.pos_x + 66, _msg.pos_y));
        network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(3, 0, 1, 0));
        network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
        network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(53, 12));
        network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
        network_register.add_component<NovaEngine::weapon_t>(id, NovaEngine::weapon_t(_msg.id, 1, _msg.pos_x, 500, "basic_weapon"));
        network_register.add_entity(id);
        send_shoot_position_to_player(id, _msg.pos_x, _msg.pos_y, client_endpoint_list, server_socket);
        return true;
    }
    if (_msg.header == 0x061A) {
        //std::cout << "Player [" << client_endpoint << "] with Id: {" << _msg.id << "} shoot bonus"  << std::endl;
        
        //crée une entité avec une vitesse constante pour le faire avancer, et weapon comme comportement
        std::size_t id = network_register.spawn_entity();
        network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(_msg.pos_x + 66, _msg.pos_y));
        network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(3, 0, 1, 0));
        network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
        network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(53, 12));
        network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
        network_register.add_component<NovaEngine::weapon_t>(id, NovaEngine::weapon_t(_msg.id, 1, _msg.pos_x, 500, "bonus_weapon_1"));
        network_register.add_entity(id);
        send_shoot_bonus1_position_to_player(id, _msg.pos_x, _msg.pos_y, client_endpoint_list, server_socket);
        return true;
    }

    if (_msg.header == 0x062A) {
        //std::cout << "Player [" << client_endpoint << "] with Id: {" << _msg.id << "} shoot bonus"  << std::endl;
        
        //crée une entité avec une vitesse constante pour le faire avancer, et weapon comme comportement
        std::size_t id = network_register.spawn_entity();
        network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(_msg.pos_x + 66, _msg.pos_y));
        network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(3, 0, 1, 0));
        network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
        network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(53, 12));
        network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
        network_register.add_component<NovaEngine::weapon_t>(id, NovaEngine::weapon_t(_msg.id, 1, _msg.pos_x, 500, "bonus_weapon_2"));
        network_register.add_entity(id);
        send_shoot_bonus2_position_to_player(id, _msg.pos_x, _msg.pos_y, client_endpoint_list, server_socket);
        return true;
    }

    if (_msg.header == 0x063A) {
        //std::cout << "Player [" << client_endpoint << "] with Id: {" << _msg.id << "} shoot bonus"  << std::endl;
        
        //crée une entité avec une vitesse constante pour le faire avancer, et weapon comme comportement
        std::size_t id = network_register.spawn_entity();
        network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(_msg.pos_x + 66, _msg.pos_y));
        network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(3, 0, 1, 0));
        network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
        network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(53, 12));
        network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
        network_register.add_component<NovaEngine::weapon_t>(id, NovaEngine::weapon_t(_msg.id, 1, _msg.pos_x, 500, "bonus_weapon_3"));
        network_register.add_entity(id);
        send_shoot_bonus3_position_to_player(id, _msg.pos_x, _msg.pos_y, client_endpoint_list, server_socket);
        return true;
    }
    if (_msg.header == 0x064A) {
        //std::cout << "Player [" << client_endpoint << "] with Id: {" << _msg.id << "} shoot bonus"  << std::endl;
        
        //crée une entité avec une vitesse constante pour le faire avancer, et weapon comme comportement
        std::size_t id = network_register.spawn_entity();
        network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(_msg.pos_x + 66, _msg.pos_y));
        network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(3, 0, 1, 0));
        network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
        network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(53, 12));
        network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
        network_register.add_component<NovaEngine::weapon_t>(id, NovaEngine::weapon_t(_msg.id, 1, _msg.pos_x, 500, "bonus_weapon_4"));
        network_register.add_entity(id);
        send_shoot_bonus3_position_to_player(id, _msg.pos_x, _msg.pos_y, client_endpoint_list, server_socket);
        return true;
    }
    if (_msg.header == 0x06B) {
        //std::cout << "Player [" << client_endpoint << "] with Id: {" << _msg.id << "} shoot bonus"  << std::endl;
        
        //crée une entité avec une vitesse constante pour le faire avancer, et weapon comme comportement
        std::size_t id = network_register.spawn_entity();
        network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(_msg.pos_x + 66, _msg.pos_y));
        network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(3, 0, 1, 0));
        network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
        network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(61, 18));
        network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
        network_register.add_component<NovaEngine::weapon_t>(id, NovaEngine::weapon_t(_msg.id, 1, _msg.pos_x, 500, "long_Shoot"));
        network_register.add_entity(id);
        send_shoot_long_position_to_player(id, _msg.pos_x, _msg.pos_y, client_endpoint_list, server_socket);
        return true;
    }
    if (_msg.header == 0x061B) {
        //std::cout << "Player [" << client_endpoint << "] with Id: {" << _msg.id << "} shoot bonus"  << std::endl;
        
        //crée une entité avec une vitesse constante pour le faire avancer, et weapon comme comportement
        std::size_t id = network_register.spawn_entity();
        network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(_msg.pos_x + 66, _msg.pos_y));
        network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(3, 0, 1, 0));
        network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
        network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(73, 18));
        network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
        network_register.add_component<NovaEngine::weapon_t>(id, NovaEngine::weapon_t(_msg.id, 1, _msg.pos_x, 500, "long_Shoot_1"));
        network_register.add_entity(id);
        send_shoot_long1_position_to_player(id, _msg.pos_x, _msg.pos_y, client_endpoint_list, server_socket);
        return true;
    }
    if (_msg.header == 0x062B) {
        //std::cout << "Player [" << client_endpoint << "] with Id: {" << _msg.id << "} shoot bonus"  << std::endl;
        
        //crée une entité avec une vitesse constante pour le faire avancer, et weapon comme comportement
        std::size_t id = network_register.spawn_entity();
        network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(_msg.pos_x + 66, _msg.pos_y));
        network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(3, 0, 1, 0));
        network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
        network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(91, 18));
        network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
        network_register.add_component<NovaEngine::weapon_t>(id, NovaEngine::weapon_t(_msg.id, 1, _msg.pos_x, 500, "long_Shoot_2"));
        network_register.add_entity(id);
        send_shoot_long2_position_to_player(id, _msg.pos_x, _msg.pos_y, client_endpoint_list, server_socket);
        return true;
    }
    if (_msg.header == 0x063B) {
        //std::cout << "Player [" << client_endpoint << "] with Id: {" << _msg.id << "} shoot bonus"  << std::endl;
        
        //crée une entité avec une vitesse constante pour le faire avancer, et weapon comme comportement
        std::size_t id = network_register.spawn_entity();
        network_register.add_component<NovaEngine::position_t>(id, NovaEngine::position_t(_msg.pos_x + 66, _msg.pos_y));
        network_register.add_component<NovaEngine::velocity_t>(id, NovaEngine::velocity_t(3, 0, 1, 0));
        network_register.add_component<NovaEngine::rigid_body_t>(id, NovaEngine::rigid_body_t(true));
        network_register.add_component<NovaEngine::box_collider_t>(id, NovaEngine::box_collider_t(180, 36));
        network_register.add_component<NovaEngine::acceleration_t>(id, NovaEngine::acceleration_t(0, 0));
        network_register.add_component<NovaEngine::weapon_t>(id, NovaEngine::weapon_t(_msg.id, 1, _msg.pos_x, 500, "long_Shoot_3"));
        network_register.add_entity(id);
        send_shoot_long3_position_to_player(id, _msg.pos_x, _msg.pos_y, client_endpoint_list, server_socket);
        return true;
    }
    return false;
}

void Player::send_shoot_position_to_player(std::size_t id, std::size_t pos_x, std::size_t pos_y,
std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
boost::asio::ip::udp::socket &server_socket)
{
    Message new_msg;
    new_msg.header = 0x06;
    new_msg.id = id;
    new_msg.pos_x = pos_x + 66;
    new_msg.pos_y = pos_y + 20;
    std::string msg = binary_protocol.crypt_message(new_msg);
    std::string compress_msg = compress_data.compressData(msg);
    for (auto client : client_endpoint_list) {
            server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
                std::bind(&Player::handle_send, this, std::placeholders::_1, "Send shoot", client));
    }
}

void Player::send_shoot_bonus1_position_to_player(std::size_t id, std::size_t pos_x, std::size_t pos_y,
std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
boost::asio::ip::udp::socket &server_socket)
{
    Message new_msg;
    new_msg.header = 0x061A;
    new_msg.id = id;
    new_msg.pos_x = pos_x + 66;
    new_msg.pos_y = pos_y + 20;
    std::string msg = binary_protocol.crypt_message(new_msg);
    std::string compress_msg = compress_data.compressData(msg);
    for (auto client : client_endpoint_list) {
            server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
                std::bind(&Player::handle_send, this, std::placeholders::_1, "Send shoot bonus", client));
    }
}


void Player::send_shoot_bonus2_position_to_player(std::size_t id, std::size_t pos_x, std::size_t pos_y,
std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
boost::asio::ip::udp::socket &server_socket)
{
    Message new_msg;
    new_msg.header = 0x062A;
    new_msg.id = id;
    new_msg.pos_x = pos_x + 66;
    new_msg.pos_y = pos_y + 20;
    std::string msg = binary_protocol.crypt_message(new_msg);
    std::string compress_msg = compress_data.compressData(msg);
    for (auto client : client_endpoint_list) {
            server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
                std::bind(&Player::handle_send, this, std::placeholders::_1, "Send shoot bonus", client));
    }
}

void Player::send_shoot_bonus3_position_to_player(std::size_t id, std::size_t pos_x, std::size_t pos_y,
std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
boost::asio::ip::udp::socket &server_socket)
{
    Message new_msg;
    new_msg.header = 0x063A;
    new_msg.id = id;
    new_msg.pos_x = pos_x + 66;
    new_msg.pos_y = pos_y + 20;
    std::string msg = binary_protocol.crypt_message(new_msg);
    std::string compress_msg = compress_data.compressData(msg);
    for (auto client : client_endpoint_list) {
            server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
                std::bind(&Player::handle_send, this, std::placeholders::_1, "Send shoot bonus", client));
    }
}


void Player::send_shoot_long_position_to_player(std::size_t id, std::size_t pos_x, std::size_t pos_y,
std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
boost::asio::ip::udp::socket &server_socket)
{
    Message new_msg;
    new_msg.header = 0x06B;
    new_msg.id = id;
    new_msg.pos_x = pos_x + 66;
    new_msg.pos_y = pos_y + 20;
    std::string msg = binary_protocol.crypt_message(new_msg);
    std::string compress_msg = compress_data.compressData(msg);
    for (auto client : client_endpoint_list) {
            server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
                std::bind(&Player::handle_send, this, std::placeholders::_1, "Send shoot bonus", client));
    }
}

void Player::send_shoot_long1_position_to_player(std::size_t id, std::size_t pos_x, std::size_t pos_y,
std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
boost::asio::ip::udp::socket &server_socket)
{
    Message new_msg;
    new_msg.header = 0x061B;
    new_msg.id = id;
    new_msg.pos_x = pos_x + 66;
    new_msg.pos_y = pos_y + 20;
    std::string msg = binary_protocol.crypt_message(new_msg);
    std::string compress_msg = compress_data.compressData(msg);
    for (auto client : client_endpoint_list) {
            server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
                std::bind(&Player::handle_send, this, std::placeholders::_1, "Send shoot bonus", client));
    }
}

void Player::send_shoot_long2_position_to_player(std::size_t id, std::size_t pos_x, std::size_t pos_y,
std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
boost::asio::ip::udp::socket &server_socket)
{
    Message new_msg;
    new_msg.header = 0x062B;
    new_msg.id = id;
    new_msg.pos_x = pos_x + 66;
    new_msg.pos_y = pos_y + 20;
    std::string msg = binary_protocol.crypt_message(new_msg);
    std::string compress_msg = compress_data.compressData(msg);
    for (auto client : client_endpoint_list) {
            server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
                std::bind(&Player::handle_send, this, std::placeholders::_1, "Send shoot bonus", client));
    }
}

void Player::send_shoot_long3_position_to_player(std::size_t id, std::size_t pos_x, std::size_t pos_y,
std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
boost::asio::ip::udp::socket &server_socket)
{
    Message new_msg;
    new_msg.header = 0x063B;
    new_msg.id = id;
    new_msg.pos_x = pos_x + 66;
    new_msg.pos_y = pos_y + 20;
    std::string msg = binary_protocol.crypt_message(new_msg);
    std::string compress_msg = compress_data.compressData(msg);
    for (auto client : client_endpoint_list) {
            server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
                std::bind(&Player::handle_send, this, std::placeholders::_1, "Send shoot bonus", client));
    }
}


void Player::handle_send(const boost::system::error_code& error, std::string txt,
boost::asio::ip::udp::endpoint client_endpoint)
{
    // if (!error) {
    //     std::cout<< "Send [" << txt  << "] to [" << client_endpoint << "] : Success" << std::endl;
        
    // } else {
    //     std::cout<< "Send [" << txt  << "] to [" << client_endpoint << "] : Failled" << std::endl;
    // }
}
