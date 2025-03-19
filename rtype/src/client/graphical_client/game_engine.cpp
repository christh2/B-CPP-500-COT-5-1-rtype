#include "../../../include/core/game.hpp"
#include "../../../include/core/graphical_client.hpp"

int main()
{
    std::unique_ptr<NovaEngine::Asset_handler> handler = std::make_unique<NovaEngine::Asset_handler>();
    std::cout << "\033[0;32mNova initializing all ressources, wait a little...\033[0m" << std::endl;
    NovaEngine::init_asset(*handler, "../assets/rtype_config/assets.json");
    Graphical_client client(handler);
    client.run();
    return 0;
}
// tu peut mettre ton main en commentaire, et mette le mien ca te permet d'avoir une idée de comment je fait
// et si besoin call moi