#include "../../include/network/server/serveur.hpp"
#include "../../../nova_engine/include/core/system.hpp"
#include "../../../nova_engine/include/core/register.hpp"

void Enemy::send_battle_ship_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register)
{
    int posx = 1905;
    int posy = rand() % 800 + 25;
//    for (int i = 0; i < 6; i++) {
        Message new_msg;
        new_msg.header = 0x09A;
        new_msg.pos_x = posx;
        new_msg.pos_y =  posy;
        std::size_t id = spawn_battle_ship(new_msg.pos_x, new_msg.pos_y, network_register);
        new_msg.id = id;
        std::string msg = binary_protocol.crypt_message(new_msg);
        std::string compress_msg = compress_data.compressData(msg);
        // std::cout << "Battle Ship [" << static_cast<int>(new_msg.id) << "] enemy created : {"
        // << new_msg.pos_x << " , " << new_msg.pos_y << "}" << std::endl;
        for (auto client : client_endpoint_list) {
            server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
            std::bind(&Enemy::handle_send, this, std::placeholders::_1, "Enemy: Battle Ship", client));
        }
        posx += 50;
        nbr_battle_ship--;
  //  }
}

void Enemy::send_papillon_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register)
{
    int posx = 2045;
    int posy = rand() % 800 + 65;
    std::size_t id = spawn_papillon_body(posx, posy, network_register);
    posx -= 45;
    posy -= 35;
    std::size_t id_1 = spawn_papillon_body(posx, posy, network_register);
    posx -= 45;
    posy -= 35;
    std::size_t id_2 = spawn_papillon_body(posx, posy, network_register);
    posx -= 45;
    posy -= 35;
    std::size_t id_3 = spawn_papillon(posx, posy, network_register);
    network_register.add_component<NovaEngine::linked_t>(id, NovaEngine::linked_t({id_1}));
    network_register.add_component<NovaEngine::linked_t>(id_1, NovaEngine::linked_t({id_2}));
    network_register.add_component<NovaEngine::linked_t>(id_2, NovaEngine::linked_t({id_3}));
    std::vector<std::size_t> gen = {id, id_1, id_2, id_3};
    for (int i = 0; i < 3; i++) {
        Message new_msg;
        new_msg.header = 0x09B;
        new_msg.pos_x = posx;
        new_msg.pos_y =  posy;
        new_msg.id = gen[i];
        std::string msg = binary_protocol.crypt_message(new_msg);
        std::string compress_msg = compress_data.compressData(msg);
        //std::cout << "pique enemy [" << static_cast<int>(new_msg.id) << "] created : {"
        //<< new_msg.pos_x << " , " << new_msg.pos_y << "}" << std::endl;
        for (auto client : client_endpoint_list) {
            server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
            std::bind(&Enemy::handle_send, this, std::placeholders::_1, "Enemy: papillon", client));
        }
    }
    nbr_papillon --;
}

void Enemy::send_commandant_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register)
{
    Message new_msg;
    new_msg.header = 0x09C;
    new_msg.pos_x = 1905;
    new_msg.pos_y =  rand() % 800;
    std::size_t id = spawn_commandant(new_msg.pos_x, new_msg.pos_y, network_register);
    new_msg.id = id;
    std::string msg = binary_protocol.crypt_message(new_msg);
    std::string compress_msg = compress_data.compressData(msg);
    //std::cout << "Commandant [" << static_cast<int>(new_msg.id) << "] enemy created : {"
    //<< new_msg.pos_x << " , " << new_msg.pos_y << "}" << std::endl;
    for (auto client : client_endpoint_list) {
        server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
        std::bind(&Enemy::handle_send, this, std::placeholders::_1, "Enemy: Commandant", client));
    }
    nbr_commandant--;
}

void Enemy::send_alien_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register)
{
    Message new_msg;
    new_msg.header = 0x091A;
    new_msg.pos_x = 1905;
    new_msg.pos_y =  rand() % 800;
    std::size_t id = spawn_alien(new_msg.pos_x, new_msg.pos_y, network_register);
    new_msg.id = id;
    std::string msg = binary_protocol.crypt_message(new_msg);
    std::string compress_msg = compress_data.compressData(msg);
    //std::cout << "pique enemy [" << static_cast<int>(new_msg.id) << "] created : {"
    //<< new_msg.pos_x << " , " << new_msg.pos_y << "}" << std::endl;
    for (auto client : client_endpoint_list) {
        server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
        std::bind(&Enemy::handle_send, this, std::placeholders::_1, "Enemy: pique", client));
    }
    nbr_alien --;
}

void Enemy::send_pique_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register)
{
    Message new_msg;
    new_msg.header = 0x091B;
    new_msg.pos_x = 1905;
    new_msg.pos_y =  rand() % 800;
    std::size_t id = spawn_pique(new_msg.pos_x, new_msg.pos_y, network_register);
    new_msg.id = id;
    std::string msg = binary_protocol.crypt_message(new_msg);
    std::string compress_msg = compress_data.compressData(msg);
    //std::cout << "pique enemy [" << static_cast<int>(new_msg.id) << "] created : {"
    //<< new_msg.pos_x << " , " << new_msg.pos_y << "}" << std::endl;
    for (auto client : client_endpoint_list) {
        server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
        std::bind(&Enemy::handle_send, this, std::placeholders::_1, "Enemy: pique", client));
    }
    nbr_pique --;
}

void Enemy::send_robot_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register)
{
    Message new_msg;
    new_msg.header = 0x091C;
    new_msg.pos_x = 1905;
    new_msg.pos_y =  rand() % 800;
    std::size_t id = spawn_robot(new_msg.pos_x, new_msg.pos_y, network_register);
    new_msg.id = id;
    std::string msg = binary_protocol.crypt_message(new_msg);
    std::string compress_msg = compress_data.compressData(msg);
    //std::cout << "pique enemy [" << static_cast<int>(new_msg.id) << "] created : {"
    //<< new_msg.pos_x << " , " << new_msg.pos_y << "}" << std::endl;
    for (auto client : client_endpoint_list) {
        server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
        std::bind(&Enemy::handle_send, this, std::placeholders::_1, "Enemy: pique", client));
    }
    nbr_robot --;
}


void Enemy::send_first_boss_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register)
{
    Message new_msg;
    new_msg.header = 0x091F;
    new_msg.pos_x = 2100;
    new_msg.pos_y =  100;
    std::size_t id = spawn_first_boss(new_msg.pos_x, new_msg.pos_y, network_register);
    new_msg.id = id;
    std::string msg = binary_protocol.crypt_message(new_msg);
    std::string compress_msg = compress_data.compressData(msg);
    //std::cout << "First boss [" << static_cast<int>(new_msg.id) << "] enemy created : {"
    //<< new_msg.pos_x << " , " << new_msg.pos_y << "}" << std::endl;
    for (auto client : client_endpoint_list) {
        //for (int a = 5; a > 0; a--) {
            server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
            std::bind(&Enemy::handle_send, this, std::placeholders::_1, "Enemy: First boss", client));
        //}
    }
    nbr_first_boss--;
}

void Enemy::send_second_boss_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register)
{
    // Création du second boss
    Message new_msg;
    new_msg.header = 0x092F;
    new_msg.pos_x = 1900;
    new_msg.pos_y = 100;
    std::size_t boss_id = spawn_second_boss(new_msg.pos_x, new_msg.pos_y, network_register);
    new_msg.id = boss_id;
    std::string msg = binary_protocol.crypt_message(new_msg);
    std::string compress_msg = compress_data.compressData(msg);

    for (auto client : client_endpoint_list) {
        server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
            std::bind(&Enemy::handle_send, this, std::placeholders::_1, "Enemy: Second boss", client));
    }
    nbr_second_boss--;

    // Création des papillons autour du boss
    const int num_papillons = 15; // Nombre de papillons autour du boss
    const float radius = 300.0f; // Rayon du cercle
    const float angle_step = 2 * M_PI / num_papillons;

    for (int i = 0; i < num_papillons; ++i) {
        float angle = i * angle_step;
        int posx = static_cast<int>(new_msg.pos_x + radius * cos(angle));
        int posy = static_cast<int>(new_msg.pos_y + radius * sin(angle));

        // Création d'un papillon
        std::size_t id = spawn_papillon_static(posx, posy, network_register);
        Message papillon_msg;
        papillon_msg.header = 0x09B;
        papillon_msg.pos_x = posx;
        papillon_msg.pos_y = posy;
        papillon_msg.id = id;

        std::string papillon_crypt_msg = binary_protocol.crypt_message(papillon_msg);
        std::string papillon_compress_msg = compress_data.compressData(papillon_crypt_msg);

        for (auto client : client_endpoint_list) {
            server_socket.async_send_to(boost::asio::buffer(papillon_compress_msg), client,
                std::bind(&Enemy::handle_send, this, std::placeholders::_1, "Enemy: Papillon", client));
        }
    }
}


void Enemy::send_third_boss_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register)
{
    Message new_msg;
    new_msg.header = 0x093F;
    new_msg.pos_x = 1900;
    new_msg.pos_y =  100;
    std::size_t id = spawn_third_boss(new_msg.pos_x, new_msg.pos_y, network_register);
    new_msg.id = id;
    std::string msg = binary_protocol.crypt_message(new_msg);
    std::string compress_msg = compress_data.compressData(msg);
    //std::cout << "Third boss [" << static_cast<int>(new_msg.id) << "] enemy created : {"
    //<< new_msg.pos_x << " , " << new_msg.pos_y << "}" << std::endl;
    for (auto client : client_endpoint_list) {
        server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
        std::bind(&Enemy::handle_send, this, std::placeholders::_1, "Enemy: Third boss", client));
    }
    nbr_third_boss --;
}


void Enemy::send_fourth_boss_enemy(std::vector<boost::asio::ip::udp::endpoint> client_endpoint_list,
boost::asio::ip::udp::socket &server_socket, NovaEngine::Registry &network_register)
{
    Message new_msg;
    new_msg.header = 0x094F;
    new_msg.pos_x = 1900;
    new_msg.pos_y =  100;
    std::size_t id = spawn_fourth_boss(new_msg.pos_x, new_msg.pos_y, network_register);
    new_msg.id = id;
    std::string msg = binary_protocol.crypt_message(new_msg);
    std::string compress_msg = compress_data.compressData(msg);
    //std::cout << "Fourth boss [" << static_cast<int>(new_msg.id) << "] enemy created : {"
    //<< new_msg.pos_x << " , " << new_msg.pos_y << "}" << std::endl;
    for (auto client : client_endpoint_list) {
        server_socket.async_send_to(boost::asio::buffer(compress_msg), client,
        std::bind(&Enemy::handle_send, this, std::placeholders::_1, "Enemy: Fourth boss", client));
    }
    nbr_fourth_boss --;
}

void UdpServer::make_level(std::string file_name, std::size_t nb_level)
{
    std::ifstream file(file_name);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << file_name << std::endl;
        return;
    }
    nlohmann::json data;
    file >> data;

    nbr_bonus = data["nbr_bonus"];
    enemy.nbr_pique = data["pique"];
    enemy.nbr_battle_ship = data["battle"];
    enemy.nbr_commandant = data["commandant"];
    enemy.nbr_alien = data["alien"];
    enemy.nbr_papillon = data["papillon"];
    enemy.nbr_robot = data["robot"];

    if (nb_level == 1) {
        enemy.nbr_first_boss = data["First_Boss"];
    }
    if (nb_level == 2) {
        enemy.nbr_second_boss = data["Second_Boss"];
    }
    if (nb_level == 3) {
        enemy.nbr_third_boss = data["Third_Boss"];
    }
    if (nb_level == 4) {
        enemy.nbr_fourth_boss = data["Fourth_Boss"];
    }
}

void UdpServer::choose_enemy_to_send()
{
    if (nbr_bonus > 0) {
        _timer_bonus.expires_after(boost::asio::chrono::seconds(12));
        _timer_bonus.async_wait(boost::bind(&UdpServer::timer_bonus, this));
    }

    if (enemy.nbr_pique > 0) {
        _timer_pique.expires_after(boost::asio::chrono::seconds(4));
        _timer_pique.async_wait(boost::bind(&UdpServer::timer_pique_enemy, this));
    }

    if (enemy.nbr_battle_ship > 0) {
        _timer_battle_ship.expires_after(boost::asio::chrono::seconds(5));
        _timer_battle_ship.async_wait(boost::bind(&UdpServer::timer_battle_ship_enemy, this));
    }

    if (enemy.nbr_commandant > 0) {
        _timer_commandant .expires_after(boost::asio::chrono::seconds(7));
        _timer_commandant.async_wait(boost::bind(&UdpServer::timer_commandant_enemy, this));
    }

    if (enemy.nbr_alien > 0) {
        _timer_alien.expires_after(boost::asio::chrono::seconds(3));
        _timer_alien.async_wait(boost::bind(&UdpServer::timer_alien_enemy, this));
    }

    if (enemy.nbr_papillon > 0) {
        _timer_papillon.expires_after(boost::asio::chrono::seconds(2));
        _timer_papillon.async_wait(boost::bind(&UdpServer::timer_papillon_enemy, this));
    }

    if (enemy.nbr_robot > 0) {
        _timer_robot.expires_after(boost::asio::chrono::seconds(6));
        _timer_robot.async_wait(boost::bind(&UdpServer::timer_robot_enemy, this));
    }

    // if (enemy.nbr_first_boss > 0) {
    //     _timer_first_boss.expires_after(boost::asio::chrono::seconds(38));
    //     _timer_first_boss.async_wait(boost::bind(&UdpServer::timer_first_boss_enemy, this));
    // }

    // if (enemy.nbr_second_boss > 0) {
        
    // }

    // if (enemy.nbr_third_boss > 0) {
        
    // }

    // if (enemy.nbr_fourth_boss > 0) {
        
    // }
}

void UdpServer::timer_enemy()
{
    if (start && start_timer == true) {
        std::cout << "Sending ennemy...\n";
        if (level == 1) {
           make_level("../level/level_1.json", 1);
        } else if (level == 2) {
            make_level("../level/level_2.json", 2);
        } else if (level == 3) {
            make_level("../level/level_3.json", 3);
        } else if (level == 4) {
            make_level("../level/level_4.json", 4);
        }

        choose_enemy_to_send();

        timer_pos_sys();
        start_timer = false;
    }
    timer_first_boss_enemy();
    timer_second_boss_enemy();
    timer_third_boss_enemy();
    timer_fourth_boss_enemy();

}

bool UdpServer::check_all_enemy_is_dead()
{
    if (enemy.nbr_alien == 0 && enemy.nbr_papillon == 0 && enemy.nbr_commandant == 0 &&
    enemy.nbr_battle_ship == 0 && enemy.nbr_robot == 0 && enemy.nbr_pique == 0) {
        return true;
    }
    return false;
}

void UdpServer::timer_battle_ship_enemy()
{
    if (enemy.nbr_battle_ship > 0) {
        //id = enemy.spawn_battle_ship(new_msg.pos_x, new_msg.pos_y, network_register);
        enemy.send_battle_ship_enemy(client_endpoint_list, server_socket, network_register);
        std::cout << "Battle Ship =========" << enemy.nbr_battle_ship << std::endl;
        _timer_battle_ship.expires_after(boost::asio::chrono::seconds(5));
        _timer_battle_ship.async_wait(boost::bind(&UdpServer::timer_battle_ship_enemy, this));
    }
}

void UdpServer::timer_pique_enemy()
{
    if (enemy.nbr_pique > 0) {
        enemy.send_pique_enemy(client_endpoint_list, server_socket, network_register);
        std::cout << "Pique =========" << enemy.nbr_pique << std::endl;
        _timer_pique.expires_after(boost::asio::chrono::seconds(4));
        _timer_pique.async_wait(boost::bind(&UdpServer::timer_pique_enemy, this));
    }
}

void UdpServer::timer_commandant_enemy()
{
    if (enemy.nbr_commandant > 0) {
        enemy.send_commandant_enemy(client_endpoint_list, server_socket, network_register);
        std::cout << "Commandant =========" << enemy.nbr_commandant << std::endl;
        _timer_commandant .expires_after(boost::asio::chrono::seconds(7));
        _timer_commandant.async_wait(boost::bind(&UdpServer::timer_commandant_enemy, this));
    }
}

void UdpServer::timer_alien_enemy()
{
    if (enemy.nbr_alien > 0) {
        enemy.send_alien_enemy(client_endpoint_list, server_socket, network_register);
        std::cout << "Alien =========" << enemy.nbr_alien << std::endl;
        _timer_alien.expires_after(boost::asio::chrono::seconds(3));
        _timer_alien.async_wait(boost::bind(&UdpServer::timer_alien_enemy, this));
    }
}

void UdpServer::timer_papillon_enemy()
{
    if (enemy.nbr_papillon > 0) {
        enemy.send_papillon_enemy(client_endpoint_list, server_socket, network_register);
        std::cout << "Papillon =========" << enemy.nbr_papillon << std::endl;
        _timer_papillon.expires_after(boost::asio::chrono::seconds(2));
        _timer_papillon.async_wait(boost::bind(&UdpServer::timer_papillon_enemy, this));
    }
}

void UdpServer::timer_robot_enemy()
{
    if (enemy.nbr_robot > 0) {
        enemy.send_robot_enemy(client_endpoint_list, server_socket, network_register);
        std::cout << "Robot =========" << enemy.nbr_robot << std::endl;
        _timer_robot.expires_after(boost::asio::chrono::seconds(6));
        _timer_robot.async_wait(boost::bind(&UdpServer::timer_robot_enemy, this));
    }
}

void UdpServer::timer_first_boss_enemy()
{
    if (enemy.nbr_first_boss > 0  && level == 1 && check_all_enemy_is_dead() == true) {
        enemy.send_first_boss_enemy(client_endpoint_list, server_socket, network_register);
        std::cout << "First Boss =========" << enemy.nbr_first_boss << std::endl;
        // _timer_first_boss.expires_after(boost::asio::chrono::seconds(38));
        // _timer_first_boss.async_wait(boost::bind(&UdpServer::timer_first_boss_enemy, this));
    }
}

void UdpServer::timer_second_boss_enemy()
{
    if (enemy.nbr_second_boss > 0 && level == 2 && check_all_enemy_is_dead() == true) {
        enemy.send_second_boss_enemy(client_endpoint_list, server_socket, network_register);
        std::cout << "Second Boss =========" << enemy.nbr_second_boss << std::endl;
        // _timer_second_boss.expires_after(boost::asio::chrono::seconds(38));
        // _timer_second_boss.async_wait(boost::bind(&UdpServer::timer_second_boss_enemy, this));
    }
}


void UdpServer::timer_third_boss_enemy()
{
    if (enemy.nbr_third_boss > 0 && level == 3 && check_all_enemy_is_dead() == true) {
        enemy.send_third_boss_enemy(client_endpoint_list, server_socket, network_register);
        std::cout << "Third Boss =========" << enemy.nbr_third_boss << std::endl;
        // _timer_third_boss.expires_after(boost::asio::chrono::seconds(38));
        // _timer_third_boss.async_wait(boost::bind(&Enemy::timer_third_boss_enemy, this));
    }
}

void UdpServer::timer_fourth_boss_enemy()
{
    if (enemy.nbr_fourth_boss > 0 && level == 4 && check_all_enemy_is_dead() == true) {
        enemy.send_fourth_boss_enemy(client_endpoint_list, server_socket, network_register);
        std::cout << "Fourth Boss =========" << enemy.nbr_fourth_boss << std::endl;
        // _timer_fourth_boss.expires_after(boost::asio::chrono::seconds(38));
        // _timer_fourth_boss.async_wait(boost::bind(&UdpServer::timer_fourth_boss_enemy, this));
    }
}



void Enemy::handle_send(const boost::system::error_code& error, std::string txt,
boost::asio::ip::udp::endpoint client_endpoint)
{
    if (!error) {
        std::cout<< "Send [" << txt  << "] to [" << client_endpoint << "] : Success" << std::endl;
        
    } else {
        std::cout<< "Send [" << txt  << "] to [" << client_endpoint << "] : Failled" << std::endl;
    }
}
