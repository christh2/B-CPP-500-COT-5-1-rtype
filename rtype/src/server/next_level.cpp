#include "../../include/network/server/serveur.hpp"

void UdpServer::next_level()
{
    std::cout << "NEXT LEVEL" << std::endl;
    level += 1;
    start_timer = true;
    
    Message new_msg;
    new_msg.header = 0x020;
    std::string msg = binary_protocol.crypt_message(new_msg);
    std::string compress_msg = compress_data.compressData(msg);
    for (auto client : client_endpoint_list) {
        server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
            std::bind(&UdpServer::handle_send, this, std::placeholders::_1, "NEXT LEVEL", client));
    }
}
