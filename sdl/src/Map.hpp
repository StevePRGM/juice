#pragma once

#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <algorithm>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "utils/min.hpp"
#include "utils/max.hpp"
#include "utils/pyth.hpp"
#include "utils/random.hpp"
#include "AABB.hpp"
#include "GlobalWindowData.hpp"
#include "CollisionBoxes.hpp"

class Player;

struct ObjectQuadsInfo {
    std::vector<int> xs;
    std::vector<int> ys;
    std::vector<int> ws;
    std::vector<int> hs;
    std::vector<int> draw_order_offset_y;
};

struct Objects {
    std::vector<int> xs;
    std::vector<int> ys;
    std::vector<int> quad_indexes;
    std::vector<float> opacities;
    std::vector<bool> draw_after_player;
};

class Map {
public:
    void LoadTexture();
    void ReloadTilesetTexture();
    void GivePlayerDelta(Player* _player, float* _dt) { player = _player; dt = _dt; };
    void CreateMapTexture();
    void CreateCollisionBoxes();
    CollisionBoxes* GetCollisionBoxes() { return & collision_boxes; }
    int GetMapWidth () { return tiles_wide * tile_length; };
    int GetMapHeight() { return tiles_high * tile_length; };
    void DrawFirst();
    void DrawSecond();
    void Update();
    void DestroyTextures();
private:
    float* dt;
    Player* player;

    const int tile_length = 16;
    const int tiles_wide = 48;
    const int tiles_high = 48;
    const int opacity_distance = 50; // minimum distance from player before objects become transparent

    CollisionBoxes collision_boxes;

    ObjectQuadsInfo object_quads_info;
    inline void AddQuad(int x, int y, int w, int h, int offset_y) {
        object_quads_info.xs.push_back(x);
        object_quads_info.ys.push_back(y);
        object_quads_info.ws.push_back(w);
        object_quads_info.hs.push_back(h);
        object_quads_info.draw_order_offset_y.push_back(offset_y);
    }

    Objects objects;
    inline void AddObject(int x, int y, int quad_index) {

        // adding with a index according to its y position so that the draw order is correct
        int index = 0;
        for (unsigned int i = 0; i < objects.ys.size(); i++) {
            if (y + object_quads_info.hs[quad_index] > objects.ys[i] + object_quads_info.hs[objects.quad_indexes[i]]) {
                index ++;
            } else {
                break;
            }
        }
        objects.xs.insert(objects.xs.begin() + index, x);
        objects.ys.insert(objects.ys.begin() + index, y);
        objects.quad_indexes.insert(objects.quad_indexes.begin() + index, quad_index);
        objects.opacities.insert(objects.opacities.begin() + index, 255);
        objects.draw_after_player.insert(objects.draw_after_player.begin() + index, true);
    }

    SDL_Rect iter_quad;
    SDL_Rect iter_pos;
    SDL_Texture* texture;
    SDL_Texture* static_saved_drawn_data = SDL_CreateTexture(global_window_data.rdr, NULL, SDL_TEXTUREACCESS_TARGET, tiles_wide * tile_length, tiles_high * tile_length);
};