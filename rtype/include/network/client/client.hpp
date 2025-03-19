#pragma once
#include "../../../include/network/server/binary_protocol.hpp"
#include "../../../include/network/server/compress_data.hpp"
#include <boost/asio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>
#include <array>
#include <unordered_map>

using boost::asio::ip::udp;


// structure pour stocker les informations d'un joueur
struct PlayerInfo {
    int id;
    int pos_x;
    int pos_y;
    int score;
};

class Client {
public:
    Client(boost::asio::io_context& io_context, const std::string& serverIP, const std::string& udpPort, const std::string username = "");
    ~Client() {}
    bool connect();
    bool authenticate(const std::string& usname, int x);
    void waitForStart();
    void sendConfirmation();
    void sendM(const std::string& mvt);
    void recevM();
    bool is_manual() { return manual; }
    void gameLoop();
    void checkForKill(const Message& receivedMessage);
    bool level_changed()  { return _next_level; }
    void reset_level() { _next_level = false; }

    // getter pour can_play
    bool isReadyToPlay() const { return can_play; }
    bool isIPvalide() const { return is_ip_valide; }
    // position du player local (moi)
    int getplayerid() const { return player_id; }
    int getPosX() const { return pos_x; }
    int getScore() const { return score; }
    int getPosY() const { return pos_y; }
    std::size_t getLives() const { return lives; }
    void setPos(int x, int y) { pos_x = x; pos_y = y;}
    std::vector<std::string> getIncomingEntities() {
        return incoming_entities;
    }
    void clear_entities() { incoming_entities.clear(); }
    std::string get_username() {return username;}

    // position d'un autre joueur (autre que le local moi)
    PlayerInfo getPlayerInfo(int id) const;
    std::unordered_map<int, PlayerInfo> getAllPlayers() const { return players; }

private:
    boost::asio::io_context& io_context;
    udp::socket udp_socket;
    udp::endpoint server_endpoint;
    udp::endpoint sender_endpoint;

    std::string serverIP;
    std::string udpPort;

    std::array<char, 1024> recv_buffer;
    std::vector<std::string> incoming_entities;
    std::string abort_message;
    bool can_play;
    int pos_x;
    int score;
    int pos_y;
    int player_id;
    bool _next_level;
    bool manual;
    int game_level;
    std::size_t lives;
    std::string username;
    bool is_ip_valide;
    bool bonus, shield, clone;
    Compress_data compress_data;
    // Map pour stocker les informations des autres joueurs
    std::unordered_map<int, PlayerInfo> players;
};

