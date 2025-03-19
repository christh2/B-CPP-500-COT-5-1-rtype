#pragma once
#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <sstream>
#include <csignal>
#include <fstream>
#include <nlohmann/json.hpp>
#include "IPlayer.hpp"
#include "IEnemy.hpp"
#include "../../../../nova_engine/include/core/hybrid_array.hpp"
#include "../../../../nova_engine/include/core/system.hpp"
#include "../../../../nova_engine/include/core/register.hpp"

/*Iterface  Iserver*/
class IServer
{
public:
    IServer() {};
    virtual  void receive_client_msg() = 0;
    virtual void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred) = 0;
    virtual void handle_send(const boost::system::error_code& error, std::string txt,
    boost::asio::ip::udp::endpoint client_endpoint) = 0;
    virtual bool connect_username(Message _msg) = 0;
    virtual bool username_position(Message _msg) = 0;
    virtual void send_start_game() = 0;
    virtual bool move(Message _msg) = 0;
    virtual void send_position_of_all_client() = 0;
    virtual void send_moving_to_all_client(Message _msg) = 0;
    virtual void timer_battle_ship_enemy() = 0;
    virtual void timer_pique_enemy() = 0;
    virtual void timer_commandant_enemy() = 0;
    virtual void timer_alien_enemy() = 0;
    virtual void timer_papillon_enemy() = 0;
    virtual void timer_robot_enemy() = 0;
    virtual void timer_first_boss_enemy() = 0;
    virtual void timer_second_boss_enemy() = 0;
    virtual void timer_third_boss_enemy() = 0;
    virtual void timer_fourth_boss_enemy() = 0;
    virtual bool deconnexion_player(Message _msg) = 0;
    virtual bool deconnexion_server() = 0;
    virtual void error_manager(Message _msg) = 0;
};

/*Class UdpServer that manage server*/
class UdpServer : IServer
{
public:
    UdpServer(boost::asio::io_context& io_context);
    ~UdpServer() {};
    void receive_client_msg();
    void handle_receive(const boost::system::error_code& error, std::size_t bytes_transferred);
    void handle_send(const boost::system::error_code& error, std::string txt,
    boost::asio::ip::udp::endpoint client_endpoint);
    void handle_signal();
    void send_start_game();
    bool send_restart_game(Message _msg);
    bool connect_username(Message _msg);
    bool get_ai_request(Message _msg);
    bool username_position(Message _msg);
    bool move(Message _msg);
    void send_position_of_all_client();
    void send_moving_to_all_client(Message _msg);
    void make_level(std::string file_name, std::size_t nb_level);
    void choose_enemy_to_send();
    bool check_all_enemy_is_dead();
    void timer_battle_ship_enemy();
    void timer_pique_enemy();
    void timer_commandant_enemy();
    void timer_alien_enemy();
    void timer_papillon_enemy();
    void timer_robot_enemy();
    void timer_first_boss_enemy();
    void timer_second_boss_enemy();
    void timer_third_boss_enemy();
    void timer_fourth_boss_enemy();
    void timer_enemy();
    void timer_run_sys();
    void timer_pos_sys();
    void timer_bonus();
    void check_dead_actor();
    void notify_death(std::size_t id_killed, std::size_t id_killer, std::size_t score);
    bool deconnexion_player(Message _msg);
    bool deconnexion_server();
    void next_level();
    void game_over();
    void error_manager(Message _msg);
    void send_bonus();
    void send_bonus_taken(std::size_t id);
private:
    Player player;
    Enemy enemy;
    std::size_t level = 1;
    std::size_t nbr_players;
    std::size_t nbr_players_game = 100;
    std::size_t nbr_bonus = 2;
    bool start = false;
    bool _next_level = false;
    bool start_timer = false;
    BinaryProtocol binary_protocol;
    Compress_data compress_data;
    boost::asio::io_context& io_context;
    NovaEngine::Registry network_register;
    std::size_t increment_pos_y = 250;
    boost::asio::ip::udp::socket server_socket;
    boost::asio::ip::udp::endpoint client_endpoint;
    boost::array<char, 1024> rev_buff;
    std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list;
    std::vector<std::size_t> client_id_list;
    boost::asio::steady_timer run_pos_timer;
    boost::asio::steady_timer run_sys_timer;
    boost::asio::steady_timer _timer_battle_ship;
    boost::asio::steady_timer _timer_pique;
    boost::asio::steady_timer _timer_commandant;
    boost::asio::steady_timer _timer_alien;
    boost::asio::steady_timer _timer_papillon;
    boost::asio::steady_timer _timer_robot;
    //boost::asio::steady_timer _timer_first_boss;
    // boost::asio::steady_timer _timer_second_boss;
    // boost::asio::steady_timer _timer_third_boss;
    // boost::asio::steady_timer _timer_fourth_boss;
    boost::asio::steady_timer _timer_bonus;
    boost::asio::signal_set signal_c;
};
