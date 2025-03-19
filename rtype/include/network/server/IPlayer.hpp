#pragma once
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <sstream>
#include "binary_protocol.hpp"
#include "compress_data.hpp"
#include "../../../../nova_engine/include/core/hybrid_array.hpp"
#include "../../../../nova_engine/include/core/system.hpp"
#include "../../../../nova_engine/include/core/register.hpp"


class IPlayer
{
    public:
        IPlayer() {};
        virtual bool move_up(Message _msg, boost::asio::ip::udp::endpoint client_endpoint, 
        NovaEngine::Registry &network_register) = 0;
        virtual bool move_down(Message _msg, boost::asio::ip::udp::endpoint client_endpoint,
        NovaEngine::Registry &network_register) = 0;
        virtual bool move_left(Message _msg, boost::asio::ip::udp::endpoint client_endpoint,
        NovaEngine::Registry &network_register) = 0;
        virtual bool move_right(Message _msg, boost::asio::ip::udp::endpoint client_endpoint,
        NovaEngine::Registry &network_register) = 0;
        virtual bool shoot(Message _msg, boost::asio::ip::udp::endpoint client_endpoint,
        NovaEngine::Registry &network_register, std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
        boost::asio::ip::udp::socket &server_socket) = 0;
        virtual  void send_shoot_position_to_player(std::size_t id, std::size_t pos_x, std::size_t pos_y,
        std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list, boost::asio::ip::udp::socket &server_socket) = 0;
};

class Player : IPlayer
{
    public:
        bool move_up(Message _msg, boost::asio::ip::udp::endpoint client_endpoint,
        NovaEngine::Registry &network_register);
        bool move_down(Message _msg, boost::asio::ip::udp::endpoint client_endpoint,
        NovaEngine::Registry &network_register);
        bool move_left(Message _msg, boost::asio::ip::udp::endpoint client_endpoint,
        NovaEngine::Registry &network_register);
        bool move_right(Message _msg, boost::asio::ip::udp::endpoint client_endpoint,
        NovaEngine::Registry &network_register);
        bool shoot(Message _msg, boost::asio::ip::udp::endpoint client_endpoint,
        NovaEngine::Registry &network_register, std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
        boost::asio::ip::udp::socket &server_socket);
        void send_shoot_position_to_player(std::size_t id, std::size_t pos_x, std::size_t pos_y,
        std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list, boost::asio::ip::udp::socket &server_socket);
        void send_shoot_bonus1_position_to_player(std::size_t id, std::size_t pos_x, std::size_t pos_y,
        std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list, boost::asio::ip::udp::socket &server_socket);
        void send_shoot_bonus2_position_to_player(std::size_t id, std::size_t pos_x, std::size_t pos_y,
        std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list, boost::asio::ip::udp::socket &server_socket);
        void send_shoot_bonus3_position_to_player(std::size_t id, std::size_t pos_x, std::size_t pos_y,
        std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list, boost::asio::ip::udp::socket &server_socket);
        void send_shoot_long_position_to_player(std::size_t id, std::size_t pos_x, std::size_t pos_y,
        std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list, boost::asio::ip::udp::socket &server_socket);
        void send_shoot_long1_position_to_player(std::size_t id, std::size_t pos_x, std::size_t pos_y,
        std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list, boost::asio::ip::udp::socket &server_socket);
        void send_shoot_long2_position_to_player(std::size_t id, std::size_t pos_x, std::size_t pos_y,
        std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list, boost::asio::ip::udp::socket &server_socket);
        void send_shoot_long3_position_to_player(std::size_t id, std::size_t pos_x, std::size_t pos_y,
        std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list, boost::asio::ip::udp::socket &server_socket);
        bool move_action = false;
        void handle_send(const boost::system::error_code& error, std::string txt,
        boost::asio::ip::udp::endpoint client_endpoint);
    private:
        BinaryProtocol binary_protocol;
        Compress_data compress_data;
};