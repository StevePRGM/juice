#pragma once

#include <time.h>

#include <SDL_CP.h>

#define CONTROLS_IMPLEMENTATION
#include "Globals/Controls.h"

#include "utils/Text.h"
#include "utils/Clock.h"
#include "utils/PrintOnScreen.h"
#include "Asset/AssetLoader.h"

#include "Globals/Window.h"

#include "Window.h"
#include "Camera.h"

#include "ECS/ECS.h"

#include "Player.h"
#include "Map.h"
#include "Enemies.h"


GlobalWindowData global_window_data = {1800, 1000, 4, NULL, NULL};

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);

    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Text::LoadFont();
    srand(time(0));

    bool DEV_PAUSED = false;

    PrintOnScreen print;
    Window window;
    Clock clock;
    Camera gameplay_camera;
    ECS ecs;
    Player player;
    Map map;
    Enemies enemies;

    clock.tick(); // avoid large dt initially

    gameplay_camera.GivePlayerMapDelta(& player, & map, & clock.dt);

    ecs.GiveMapPlayerEnemies(& map, & player, & enemies);

    player.LoadAsset();
    player.GiveMapEnemiesECSDelta(& map, & enemies, & ecs, & clock.dt);
    player.InitPos();

    map.LoadTexture();
    map.GivePlayerDeltaECS(& player, & clock.dt, & ecs);
    map.CreateMapTexture();
    map.CreateCollisionBoxes();

    enemies.CreateTextures();
    enemies.GiveDeltaMapECSPlayer(& clock.dt, & map, & ecs, & player);
    enemies.CreateEnemies();

    SDL_Event event;
    bool quit = false;
    while (!quit) {

        // UPDATE

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            quit = true;
                            break;
                        case SDLK_LCTRL:
                            DEV_PAUSED = ! DEV_PAUSED;
                            break;
                        case SDLK_r:
                            map.ReloadTilesetTexture();
                            player.DestroyAsset();
                            player.LoadAsset();
                            break;
                        default: break;
                    }
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        global_window_data.w = event.window.data1;
                        global_window_data.h = event.window.data2;
                    }
                    break;
                default: break;
            }
        }

        clock.tick();

        if (DEV_PAUSED) {
            gameplay_camera.DevUpdate();
        } else {
            player.Update();
            map.Update();
            enemies.Update();
            gameplay_camera.Update();
        }

        // DRAW

        window.Clear();

        window.SetDrawGameplay();

        map.DrawBase();
        ecs.Draw();

        window.SetDrawOther();

        print.Draw(std::to_string( clock.average_fps ), 2, 0);

        window.Present(gameplay_camera.GetViewport());
    }

    player.DestroyAsset();
    map.DestroyTextures();

    Text::DestroyFont();
    TTF_Quit();
    IMG_Quit();
    window.Shutdown();
    SDL_Quit();
    return 0;
}