#include <iostream>
#include "../../include/network/server/serveur.hpp"

/*This function is the main where serveur start with io_context.run()*/

int main()
{
    try
    {
        boost::asio::io_context io_context;
        UdpServer server(io_context);
        srand(time(NULL));
        while (1) {
            io_context.run_for(std::chrono::milliseconds(20));
            //server.timer_run_sys();
            server.check_dead_actor();
        }
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}

/*
    Indices :
    prédiction côté client avec réconciliation côté serveur,
    mises à jour serveur à faible fréquence avec interpolation de l'état de l'entité,
    compensation du lag côté serveur,
    retard d'entrée,
    rollback netcode.

*/