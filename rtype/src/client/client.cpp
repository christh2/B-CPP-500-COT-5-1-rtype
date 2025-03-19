#include "../../include/network/client/client.hpp"
#include <boost/asio.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <string>
#include <array>
#include <unordered_set>

using boost::asio::ip::udp;

Client::Client(boost::asio::io_context& io_context, const std::string& serverIP, const std::string& udpPort, const std::string usr_name)
    : io_context(io_context), udp_socket(io_context, udp::endpoint(boost::asio::ip::udp::v4(), 0)), can_play(false), pos_x(0), pos_y(0), is_ip_valide(false), username(usr_name)
{
    try {
        boost::asio::ip::address ip = boost::asio::ip::make_address(serverIP);
        server_endpoint = udp::endpoint(ip, std::stoi(udpPort));
        is_ip_valide = true;
        score = 0;
        lives = 3;
        game_level = 0;
        _next_level = false;
        bonus = false;
        clone = false;
        shield = false;
        manual = true;
        abort_message = "";
    } catch (const std::exception& e) {
        std::cerr << "Erreur : l'IP (" << serverIP << ") n'est pas valide." << std::endl;
        is_ip_valide = false;
        throw;
    }
}

bool Client::connect() {
    BinaryProtocol binaryProtocol;
    Message connectMessage;
    connectMessage.header = 0x01;
    connectMessage.pos_x = pos_x;
    connectMessage.pos_y = pos_y;

    std::string encryptedMessage = binaryProtocol.crypt_message(connectMessage);
    std::string compressedMessage = compress_data.compressData(encryptedMessage);  // Compression du message connction
    udp_socket.async_send_to(boost::asio::buffer(compressedMessage), server_endpoint,
        [](boost::system::error_code, std::size_t) {});

    recevM();
    return true;
}

bool Client::authenticate(const std::string& usname, int x) {
    BinaryProtocol binaryProtocol;
    Message authMessage;
    authMessage.header = 0x02;  // header d'authentification
    authMessage.pos_x = x;
    std::cout << "Nb players: " << x << std::endl;
    authMessage.pos_y = pos_y;

    std::string encryptedMessage = binaryProtocol.crypt_message(authMessage);
    std::string compressedMessage = compress_data.compressData(encryptedMessage);
    udp_socket.async_send_to(boost::asio::buffer(compressedMessage), server_endpoint,
        [](boost::system::error_code, std::size_t) {});

    recevM();
    return true;
}

void Client::checkForKill(const Message& receivedMessage) {
    std::string vlue = "Killed " + std::to_string((int) receivedMessage.id);
    incoming_entities.push_back(vlue);
    if (receivedMessage.header == 0x07 && receivedMessage.id == player_id) {
        //std::cout << "Vous avez été touché !" << std::endl;
    } else if (receivedMessage.header == 0x07 && receivedMessage.id != player_id) {
        if (receivedMessage.pos_x == player_id) {
            score = receivedMessage.pos_y;
            players[player_id].score = score;
            return;
        }
        for (auto &it : players) {
            if (it.second.id == receivedMessage.pos_x) {
                it.second.score = receivedMessage.pos_y;
                std::cout << receivedMessage.pos_y;
                return;
            }
        }
    }
}

void Client::recevM() {

    static const std::unordered_set<int> valid_headers = { 
        0x03, 0x07, 0x051,
        0x010, 0x011, 0x020,
        0x04, 0x06, 0x09A,
        0x09B, 0x09C, 0x09D,
        0x09F, 0x099, 0x019A,
        0x019B, 0x06A, 0x015,
        0x091A, 0x091B, 0x091F,
        0x092F, 0x093F, 0x094F,
        0x06B, 0x061B, 0x062B,
        0x061A, 0x063B, 0x062A,
        0x063A, 0x064A, 0x091C
    };

    udp_socket.async_receive_from(
        boost::asio::buffer(recv_buffer), sender_endpoint,
        [this](boost::system::error_code ec, std::size_t bytes_recvd) {
            if (!ec && bytes_recvd > 0) {
                BinaryProtocol binaryProtocol;
                std::string receivedData(recv_buffer.data(), bytes_recvd);
                std::string decompressedData = compress_data.decompressData(receivedData);  // decompression des données
                Message receivedMessage = binaryProtocol.decrypt_message(decompressedData);  // désérialisation du message

                // Vérifier si le header est valide
                if (valid_headers.find(receivedMessage.header) == valid_headers.end()) {
                    //std::cerr << "Erreur : header invalide reçu (" << std::hex << receivedMessage.header << ")" << std::endl;
                } else { 
                    if (receivedMessage.header == 0x03) {
                        player_id = receivedMessage.id;
                        pos_x = receivedMessage.pos_x;
                        pos_y = receivedMessage.pos_y;
                        PlayerInfo otherPlayer;
                        otherPlayer.id = receivedMessage.id;
                        otherPlayer.pos_x = receivedMessage.pos_x;
                        otherPlayer.pos_y = receivedMessage.pos_y;
                        otherPlayer.score = 0;
                        players[otherPlayer.id] = otherPlayer;
                    }
                else if (receivedMessage.header == 0x07) {
                    //std::cout << "someone is killed" << std::endl;
                    checkForKill(receivedMessage);
                }
                else if (receivedMessage.header == 0x051) {
                    // Mise à jour de la position d'un autre joueur
                    if (players.find(receivedMessage.id) != players.end()) {
                        players[receivedMessage.id].pos_x = receivedMessage.pos_x;
                        players[receivedMessage.id].pos_y = receivedMessage.pos_y;
                        if (receivedMessage.id == player_id) {
                            pos_x = receivedMessage.pos_x;
                            pos_y = receivedMessage.pos_y;
                        }
                    } else {
                        PlayerInfo otherPlayer;
                        otherPlayer.id = receivedMessage.id;
                        otherPlayer.pos_x = receivedMessage.pos_x;
                        otherPlayer.pos_y = receivedMessage.pos_y;
                        otherPlayer.score = 0;
                        players[otherPlayer.id] = otherPlayer;
                    }
                }
                else if (receivedMessage.header == 0x010) {
                    can_play = false;
                    abort_message = "A player is gone";
                } else if (receivedMessage.header == 0x011) {
                    can_play = false;
                    abort_message = "Server no longer available";
                } else if (receivedMessage.header == 0x020) {
                    std::cout << "Next Level\n"; 
                    _next_level = true;
                    game_level += 1;
                }
                else if (receivedMessage.header == 0x04) {
                    can_play = true;
                    //sendConfirmation();
                  //  std::cout << "Début du jeu autorisé !" << std::endl;
                } else if (receivedMessage.header == 0x06) {
                    can_play = true;
                    std::string vlue = "Weapon " + std::to_string((int) receivedMessage.id) + " " +
                        std::to_string(receivedMessage.pos_x) + " " + std::to_string(receivedMessage.pos_y);
                    incoming_entities.push_back(vlue);
                } else if (receivedMessage.header == 0x09A) {
                    can_play = true;
                    std::string vlue = "Enemy Battle " + std::to_string((int) receivedMessage.id) + " " +
                        std::to_string(receivedMessage.pos_x) + " " + std::to_string(receivedMessage.pos_y);
                    incoming_entities.push_back(vlue);
                } else if (receivedMessage.header == 0x09B) {
                    can_play = true;
                    std::string vlue = "Enemy Papillon " + std::to_string((int) receivedMessage.id) + " " +
                        std::to_string(receivedMessage.pos_x) + " " + std::to_string(receivedMessage.pos_y);
                    incoming_entities.push_back(vlue);
                } else if (receivedMessage.header == 0x09C) {
                    can_play = true;
                    std::string vlue = "Enemy Commandant " + std::to_string((int) receivedMessage.id) + " " +
                        std::to_string(receivedMessage.pos_x) + " " + std::to_string(receivedMessage.pos_y);
                    incoming_entities.push_back(vlue);
                } else if (receivedMessage.header == 0x09D) {
                } else if (receivedMessage.header == 0x09F) {
                    can_play = true;
                    std::string vlue = "Enemy Crevette " + std::to_string(receivedMessage.id) + " " +
                        std::to_string(receivedMessage.pos_x) + " " + std::to_string(receivedMessage.pos_y);
                    incoming_entities.push_back(vlue);
                } else if (receivedMessage.header == 0x091A) {
                    can_play = true;
                    std::string vlue = "Enemy Alien " + std::to_string(receivedMessage.id) + " " +
                        std::to_string(receivedMessage.pos_x) + " " + std::to_string(receivedMessage.pos_y);
                    incoming_entities.push_back(vlue);
                } else if (receivedMessage.header == 0x091B) {
                    can_play = true;
                    std::string vlue = "Enemy Pique " + std::to_string(receivedMessage.id) + " " +
                        std::to_string(receivedMessage.pos_x) + " " + std::to_string(receivedMessage.pos_y);
                    incoming_entities.push_back(vlue);
                } else if (receivedMessage.header == 0x091C) {
                    can_play = true;
                    std::string vlue = "Enemy Robot " + std::to_string(receivedMessage.id) + " " +
                        std::to_string(receivedMessage.pos_x) + " " + std::to_string(receivedMessage.pos_y);
                    incoming_entities.push_back(vlue);
                } else if (receivedMessage.header == 0x091F) {
                    can_play = true;
                    std::string vlue = "Enemy First " + std::to_string(receivedMessage.id) + " " +
                        std::to_string(receivedMessage.pos_x) + " " + std::to_string(receivedMessage.pos_y);
                    incoming_entities.push_back(vlue);
                } else if (receivedMessage.header == 0x092F) {
                    can_play = true;
                    std::string vlue = "Enemy Second " + std::to_string(receivedMessage.id) + " " +
                        std::to_string(receivedMessage.pos_x) + " " + std::to_string(receivedMessage.pos_y);
                    incoming_entities.push_back(vlue);
                } else if (receivedMessage.header == 0x093F) {
                    can_play = true;
                   std::string vlue = "Enemy Third " + std::to_string(receivedMessage.id) + " " +
                        std::to_string(receivedMessage.pos_x) + " " + std::to_string(receivedMessage.pos_y);
                    incoming_entities.push_back(vlue);
                } else if (receivedMessage.header == 0x094F) {
                    can_play = true;
                    std::string vlue = "Enemy Fourth " + std::to_string(receivedMessage.id) + " " +
                        std::to_string(receivedMessage.pos_x) + " " + std::to_string(receivedMessage.pos_y);
                    incoming_entities.push_back(vlue);
                } else if (receivedMessage.header == 0x019A) {
                    std::string vlue = "Bonus " + std::to_string((int) receivedMessage.id) + " " +
                        std::to_string(receivedMessage.pos_x) + " " + std::to_string(receivedMessage.pos_y);
                    incoming_entities.push_back(vlue);
                } else if (receivedMessage.header == 0x099) {
                    std::string vlue = "Attack 0 " + std::to_string((int) receivedMessage.id) + " " +
                        std::to_string(receivedMessage.pos_x) + " " + std::to_string(receivedMessage.pos_y);
                    incoming_entities.push_back(vlue);
                } else if (receivedMessage.header == 0x061A) {
                    std::string vlue = "Bonus_shoot 0 " + std::to_string((int) receivedMessage.id) + " " +
                        std::to_string(receivedMessage.pos_x) + " " + std::to_string(receivedMessage.pos_y);
                    incoming_entities.push_back(vlue);
                } else if (receivedMessage.header == 0x062A) {
                    std::string vlue = "Bonus_shoot 1 " + std::to_string((int) receivedMessage.id) + " " +
                        std::to_string(receivedMessage.pos_x) + " " + std::to_string(receivedMessage.pos_y);
                    incoming_entities.push_back(vlue);
                } else if (receivedMessage.header == 0x063A) {
                    std::string vlue = "Bonus_shoot 2 " + std::to_string((int) receivedMessage.id) + " " +
                        std::to_string(receivedMessage.pos_x) + " " + std::to_string(receivedMessage.pos_y);
                    incoming_entities.push_back(vlue);
                } else if (receivedMessage.header == 0x064A) {
                    std::string vlue = "Bonus_shoot 3 " + std::to_string((int) receivedMessage.id) + " " +
                        std::to_string(receivedMessage.pos_x) + " " + std::to_string(receivedMessage.pos_y);
                    incoming_entities.push_back(vlue);
                } else if (receivedMessage.header == 0x06B) {
                    std::string vlue = "Attack 1 " + std::to_string((int) receivedMessage.id) + " " +
                        std::to_string(receivedMessage.pos_x) + " " + std::to_string(receivedMessage.pos_y);
                    incoming_entities.push_back(vlue);
                } else if (receivedMessage.header == 0x061B) {
                    std::string vlue = "Attack 2 " + std::to_string((int) receivedMessage.id) + " " +
                        std::to_string(receivedMessage.pos_x) + " " + std::to_string(receivedMessage.pos_y);
                    incoming_entities.push_back(vlue);
                } else if (receivedMessage.header == 0x062B) {
                    std::string vlue = "Attack 3 " + std::to_string((int) receivedMessage.id) + " " +
                        std::to_string(receivedMessage.pos_x) + " " + std::to_string(receivedMessage.pos_y);
                    incoming_entities.push_back(vlue);
                } else if (receivedMessage.header == 0x063B) {
                    std::string vlue = "Attack 4 " + std::to_string((int) receivedMessage.id) + " " +
                        std::to_string(receivedMessage.pos_x) + " " + std::to_string(receivedMessage.pos_y);
                    incoming_entities.push_back(vlue);
                } else if (receivedMessage.header == 0x019B) {
                    bonus = true;
                } else if (receivedMessage.header == 0x015) {
                    can_play = false;
                }
            }
            // Relancer la réception pour le prochain message
            recevM();
        }
    });
}

void Client::waitForStart() {
    while (!can_play) {
        io_context.poll();
    }
}

void Client::sendConfirmation() {
    BinaryProtocol binaryProtocol;
    Message confirmationMessage;
    confirmationMessage.header = 0x04A; // Confirmation de start
    confirmationMessage.id = player_id;

    std::string encryptedMessage = binaryProtocol.crypt_message(confirmationMessage);
    std::string compressedMessage = compress_data.compressData(encryptedMessage);
    udp_socket.async_send_to(boost::asio::buffer(compressedMessage), server_endpoint,
        [](boost::system::error_code, std::size_t) {});
}


void Client::sendM(const std::string& mvt) {
    BinaryProtocol binaryProtocol;
    Message moveMessage;
    moveMessage.id = player_id; // j'ajoute son id a chaque mvt que je send au server (omer)

    if (mvt == "up")
        moveMessage.header = 0x05A;
    else if (mvt == "down")
        moveMessage.header = 0x05B;
    else if (mvt == "left")
        moveMessage.header = 0x05C;
    else if (mvt == "right")
        moveMessage.header = 0x05D;
    else if (mvt == "shoot" && bonus)
        moveMessage.header = 0x061A;
    else if (mvt == "shoot" && !bonus)
        moveMessage.header = 0x06;
    else if (mvt == "long shoot 0" && !bonus) {
        moveMessage.header = 0x06B;
    } else if (mvt == "long shoot 1" && !bonus)
        moveMessage.header = 0x061B;
    else if (mvt == "long shoot 2" && !bonus)
        moveMessage.header = 0x062B;
    else if (mvt == "long shoot 3" && !bonus)
        moveMessage.header = 0x063B;
    else if (mvt == "long shoot 0" && bonus)
        moveMessage.header = 0x061A;
    else if (mvt == "long shoot 1" && bonus)
        moveMessage.header = 0x062A;
    else if (mvt == "long shoot 2" && bonus)
        moveMessage.header = 0x063A;
    else if (mvt == "long shoot 3" && bonus)
        moveMessage.header = 0x064A;
    else if (mvt == "bye")
        moveMessage.header = 0x010;
    else if (mvt == "auto") {
        if (manual)
            moveMessage.header = 0x08A;
        else
            moveMessage.header = 0x08B;
        manual = !manual;
    }
    moveMessage.pos_x = pos_x;
    moveMessage.pos_y = pos_y;

    std::string encryptedMessage = binaryProtocol.crypt_message(moveMessage);
    std::string compressedMessage = compress_data.compressData(encryptedMessage);
    udp_socket.async_send_to(boost::asio::buffer(compressedMessage), server_endpoint,
        [](boost::system::error_code, std::size_t) {});
}
