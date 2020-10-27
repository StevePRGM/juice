#include "Enemies.h"
#include "Player.h"
#include "Map.h"
#include "ECS/ECS.h"

void Enemies::CreateTextures() {
   spider_texture = LoadImage(global_window_data.rdr, "assets/enemies/spider.png");
}

void Enemies::GiveDeltaMapECSPlayer(float* _dt, Map* _map, ECS* _ecs, Player* _player) {
    dt     = _dt;
    map    = _map;
    ecs    = _ecs;
    player = _player;
}

void Enemies::CreateEnemies() {
    for (int i = 0; i < 5; i ++) {

        // int id = ecs->AddEntity(random(50, map->map_width - 100), random(50, map->map_height - 100), SPIDER, ENEMY_TYPE);
        int id = ecs->AddEntity(50, 50, SPIDER, ENEMY_TYPE);
        float tick = float(random(0, 200)) / 1000;
        enemies[id] = {false, {IDLE, tick, 0}};
    }
}

void Enemies::DrawEnemy(int id) {

    SDL_Rect quad;
    SDL_Rect pos;

    UpdateAnimationQuad(ecs->entities[id].name, & enemies[id].cur_anim, & quad.x, & quad.y);
    quad.w = pos.w = QUAD_DIMENSIONS[ecs->entities[id].name].w;
    quad.h = pos.h = QUAD_DIMENSIONS[ecs->entities[id].name].h;
    pos.x = ecs->entities[id].x;
    pos.y = ecs->entities[id].y;

    SDL_RenderCopyEx(global_window_data.rdr, spider_texture, & quad, & pos, NULL, NULL, enemies[id].is_right ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
}

void Enemies::Update() {
    // it.first  -> entity id
    // it.second -> Enemy struct with additional components

    for (auto & it: enemies) {
        UpdateEnemyAnimation(it.first, & it.second);
        UpdateEnemyMovement( it.first, & it.second);
    }


}

void Enemies::UpdateEnemyAnimation(int id, Enemy* enemy) {
    int distance_from_player_squared = pyth_s(ecs->GetCenterX(id), ecs->GetCenterY(id), ecs->GetCenterX(player->id), ecs->GetCenterY(player->id));

    if (distance_from_player_squared < attack_distance*attack_distance) {
        SetAnimationIf(ecs->entities[id].name, & enemy->cur_anim, ATTACK);
    }
    else if (distance_from_player_squared < activation_distance*activation_distance) {
        SetAnimationIf(ecs->entities[id].name, & enemy->cur_anim, RUN);
    }
    else if (distance_from_player_squared > deactivation_distance*deactivation_distance) {
        SetAnimationIf(ecs->entities[id].name, & enemy->cur_anim, IDLE);
    }

    AnimationTick(ecs->entities[id].name, & enemy->cur_anim, dt);
}

void Enemies::UpdateEnemyMovement(int id, Enemy* enemy) {

    if (enemy->cur_anim.type == RUN || enemy->cur_anim.type == ATTACK) {

        // Calculate xv and yv for enemy to follow player at a max speed and at the correct angle.

        float xv = ecs->GetCenterX(player->id) - ecs->GetCenterX(id);
        float yv = ecs->GetCenterY(player->id) - ecs->GetCenterY(id);

        enemy->is_right = xv > 0 ? true : false;

        if (enemy->cur_anim.type == RUN) { // To avoid vibration when next to the player.

            float cap_v = 50 / sqrt(xv*xv + yv*yv);

            xv *= cap_v;
            yv *= cap_v;


            ecs->entities[id].x += xv * (*dt);
            ecs->entities[id].y += yv * (*dt);

        }
    }
}
