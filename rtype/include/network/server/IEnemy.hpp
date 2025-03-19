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


class IEnnemy
{
    public:
        IEnnemy() {};
        virtual std::size_t spawn_battle_ship(int x, int y, NovaEngine::Registry &network_register) = 0;
        virtual std::size_t spawn_pique(int x, int y, NovaEngine::Registry &network_register) = 0;
        virtual std::size_t spawn_commandant(int x, int y, NovaEngine::Registry &network_register) = 0;
        virtual std::size_t spawn_first_boss(int x, int y, NovaEngine::Registry &network_register) = 0;
        virtual void send_battle_ship_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
        boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register) = 0;
        virtual void send_pique_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
        boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register) = 0;
        virtual void send_commandant_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
        boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register) = 0;
        virtual void send_first_boss_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
        boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register) = 0;
};

class Enemy : IEnnemy
{
    public:
        std::size_t spawn_battle_ship(int x, int y, NovaEngine::Registry &network_register);
        std::size_t spawn_pique(int x, int y, NovaEngine::Registry &network_register);
        std::size_t spawn_commandant(int x, int y, NovaEngine::Registry &network_register);
        std::size_t spawn_alien(int x, int y, NovaEngine::Registry &network_register);
        std::size_t spawn_papillon(int x, int y, NovaEngine::Registry &network_register);
        std::size_t spawn_papillon_body(int x, int y, NovaEngine::Registry &network_register);
        std::size_t spawn_papillon_static(int x, int y, NovaEngine::Registry &network_register);
        std::size_t spawn_robot(int x, int y, NovaEngine::Registry &network_register);
        std::size_t spawn_robot_2(int x, int y, NovaEngine::Registry &network_register);
        std::size_t spawn_first_boss(int x, int y, NovaEngine::Registry &network_register);
        std::size_t spawn_second_boss(int x, int y, NovaEngine::Registry &network_register);
        std::size_t spawn_third_boss(int x, int y, NovaEngine::Registry &network_register);
        std::size_t spawn_fourth_boss(int x, int y, NovaEngine::Registry &network_register);
        void send_battle_ship_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
        boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register);
        void send_pique_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
        boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register);
        void send_commandant_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
        boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register);
        void send_alien_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
        boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register);
        void send_papillon_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
        boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register);
        void send_robot_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
        boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register);
        void send_first_boss_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
        boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register);
        void send_second_boss_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
        boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register);
        void send_third_boss_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
        boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register);
        void send_fourth_boss_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
        boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register);
        void send_enemy_shoot(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
        boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register);
        void handle_send(const boost::system::error_code& error, std::string txt,
        boost::asio::ip::udp::endpoint client_endpoint);
        std::size_t nbr_battle_ship = 0;
        std::size_t nbr_commandant = 0;
        std::size_t nbr_alien = 0;
        std::size_t nbr_papillon = 0;
        std::size_t nbr_pique = 0;
        std::size_t nbr_robot = 0;
        std::size_t nbr_first_boss = 0;
        std::size_t nbr_second_boss = 0;
        std::size_t nbr_third_boss = 0;
        std::size_t nbr_fourth_boss = 0;
    private:
        BinaryProtocol binary_protocol;
        Compress_data compress_data;
};
