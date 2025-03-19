#include "../../include/network/server/serveur.hpp"

void UdpServer::error_manager(Message _msg)
{
    Message error_msg;
    error_msg.header = 0x000;
    std::cout << "Error message :" << _msg.header << "from palyer : " << client_endpoint << std::endl;
    std::string msg = binary_protocol.crypt_message(error_msg);
    std::string compress_msg = compress_data.compressData(msg);
    server_socket.async_send_to(boost::asio::buffer(compress_msg), client_endpoint,
        std::bind(&UdpServer::handle_send, this, std::placeholders::_1, "Error messsage", client_endpoint));
}
