#pragma once

#include <algorithm>

#include <SDL_CP.h>

#include "../Globals/All.h"
#include "Entity.h"
#include "../Asset/AssetLoader.h"

struct Map;
struct Player;
struct Enemies;

struct ECS {

    void GivePointers(Map* _map, Player* _player, Enemies* _enemies);
    int AddEntity(ENTITY_NAME name, ENTITY_TYPE type, float x, float y, Asset_Ase* asset = NULL);
    void PopEntity(int id);

    inline float GetCenterX(int id) { return entities[id].x + COLLISION_DATA[entities[id].name].x + COLLISION_DATA[entities[id].name].w / 2; }
    inline float GetCenterY(int id) { return entities[id].y + COLLISION_DATA[entities[id].name].y + COLLISION_DATA[entities[id].name].h / 2; }

    void Draw();

    Map* map;
    Player* player;
    Enemies* enemies;

    std::unordered_map<int, Entity> entities;
    std::vector<int> draw_order_indexes; // order they're drawn in.
};