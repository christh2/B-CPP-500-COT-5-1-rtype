#include "../../include/network/server/serveur.hpp"
#include "../../../nova_engine/include/core/system.hpp"
#include "../../../nova_engine/include/core/register.hpp"

bool UdpServer::move(Message _msg)
{
    auto &behave = network_register.get_component<NovaEngine::behavior_t>(_msg.id);
    if (behave.has_value())
        return false;
    if (player.move_up(_msg, client_endpoint, network_register)) {
        send_moving_to_all_client(_msg);
        return true;
    }
    if (player.move_down(_msg, client_endpoint, network_register)) {
        send_moving_to_all_client(_msg);
        return true;
    }
    if (player.move_left(_msg, client_endpoint, network_register)) {
        send_moving_to_all_client(_msg);
        return true;
    }
    if (player.move_right(_msg, client_endpoint, network_register)) {
        send_moving_to_all_client(_msg);
        return true;
    }
    return false;
}

bool Player::move_up(Message _msg, boost::asio::ip::udp::endpoint client_endpoint,
NovaEngine::Registry &network_register)
{
    if (_msg.header == 0x05A && !network_register.get_component<NovaEngine::behavior_t>(_msg.id).has_value()) {
     //   std::cout << "Player [" << client_endpoint << "] with  Id: {" << static_cast<int>(_msg.id) << "} go Up"  << std::endl;
        NovaEngine::position_t &pos = network_register.get_component<NovaEngine::position_t>(_msg.id).value();
        pos._x = _msg.pos_x;
        pos._y = _msg.pos_y;
        return true;
    }
    return false;
}

bool Player::move_down(Message _msg, boost::asio::ip::udp::endpoint client_endpoint,
NovaEngine::Registry &network_register)
{
    if (_msg.header == 0x05B && !network_register.get_component<NovaEngine::behavior_t>(_msg.id).has_value()) {
      //  std::cout << "Player [" << client_endpoint << "] with  Id: {" << static_cast<int>(_msg.id) << "} go Down"  << std::endl;
        NovaEngine::position_t &pos = network_register.get_component<NovaEngine::position_t>(_msg.id).value();
        pos._x = _msg.pos_x;
        pos._y = _msg.pos_y;
        return true;
    }
    return false;
}
bool Player::move_left(Message _msg, boost::asio::ip::udp::endpoint client_endpoint,
NovaEngine::Registry &network_register)
{
   if (_msg.header == 0x05C && !network_register.get_component<NovaEngine::behavior_t>(_msg.id).has_value()) {
      //  std::cout << "Player [" << client_endpoint << "] with  Id: {" << static_cast<int>(_msg.id) << "} go Left"  << std::endl;
        NovaEngine::position_t &pos = network_register.get_component<NovaEngine::position_t>(_msg.id).value();
        pos._x = _msg.pos_x;
        pos._y = _msg.pos_y;
        return true;
    }
    return false;
}
bool Player::move_right(Message _msg, boost::asio::ip::udp::endpoint client_endpoint,
NovaEngine::Registry &network_register)
{
    if (_msg.header == 0x05D && !network_register.get_component<NovaEngine::behavior_t>(_msg.id).has_value()) {
      //  std::cout << "Player [" << client_endpoint << "] with  Id: {" << static_cast<int>(_msg.id) << "} go Right"  << std::endl;
        NovaEngine::position_t &pos = network_register.get_component<NovaEngine::position_t>(_msg.id).value();
        pos._x = _msg.pos_x;
        pos._y = _msg.pos_y;
        return true;
    }
    return false;
}

void UdpServer::send_moving_to_all_client(Message _msg)
{
    for (auto client : client_endpoint_list) {
        Message new_msg;
        new_msg.header = 0x051;
        new_msg.id = _msg.id;
        new_msg.pos_x = _msg.pos_x;
        new_msg.pos_y = _msg.pos_y;
        std::string msg = binary_protocol.crypt_message(new_msg);
        std::string compress_msg = compress_data.compressData(msg);
        server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
            std::bind(&UdpServer::handle_send, this, std::placeholders::_1, "Send all position", client));
    }
}
