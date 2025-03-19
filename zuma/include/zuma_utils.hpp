#pragma once
#include "../../nova_engine/include/core/register.hpp"
#include "../../nova_engine/include/core/asset_handler.hpp"
#include "../../nova_engine/include/core/entity.hpp"
#include "../../nova_engine/include/core/core_utils.hpp"

typedef enum path {
    MENU,
    OPTIONS,
    SHOP,
    LEVELS,
    GAME,
    CHOICE,
    QUIT
} path_t;