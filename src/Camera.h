#pragma once

#include <SDL_CP.h>

#include "Globals/All.h"

#include "Player.h"
#include "Map.h"
#include "utils/extramath.h"

struct Camera {
    void PassPointers(Player* _player, Map* _map, float* _dt);
    void Update();
    void DevUpdate();

    Player* player;
    Map* map;
    float* dt;
    float real_x = 0;
    float real_y = 0;
    int max_distance_from_player = 30;
    float pan_v = 400;
};