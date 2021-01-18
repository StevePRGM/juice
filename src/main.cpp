#pragma once

#include <time.h>
#include <stdio.h>

#include <SDL_CP.h>

#include "utils/Text.h"

#define CONTROLS_IMPLEMENTATION
#include "utils/Controls.h"

#include "utils/Clock.h"
#include "utils/PrintScreen.h"
#include "Asset/AssetLoader.h"

#include "Globals/Window.h"

#include "Window.h"
#include "Camera.h"

#include "ECS/ECS.h"

#include "Player.h"
#include "Map.h"
#include "Enemies.h"


GlobalWindowData g_window = {1800, 1000, 4, NULL, NULL};

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    srand(time(0));
    Text::LoadFont();
    CTS::Init();

    bool DEV_PAUSED = false;

    Window window;
    Clock clock;
    Camera gameplay_camera;
    ECS ecs;
    Player player;
    Map map;
    Enemies enemies;

    clock.tick(); // avoid large dt initially

    gameplay_camera.PassPointers(& player, & map, & clock.dt);

    ecs.GivePointers(& map, & player, & enemies);

    player.LoadAsset();
    player.PassPointers(& map, & enemies, & ecs, & clock.dt);
    player.InitPos();

    map.LoadAssets();
    map.PassPointers(& player, & clock.dt, & ecs);
    map.CreateMapTexture();
    map.CreateCollisionBoxes();

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
                        case SDLK_r:
                            map.DestroyAssets();
                            map.LoadAssets();
                            player.DestroyAsset();
                            player.LoadAsset();
                            break;
                        default: break;
                    }
                case SDL_WINDOWEVENT:
                    if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                        g_window.w = event.window.data1;
                        g_window.h = event.window.data2;
                    }
                    break;
                default: break;
            }
        }

        if (CTS::ActionDev()) DEV_PAUSED = ! DEV_PAUSED;

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

        PrintScreen(std::to_string( clock.average_fps ), 2, 0);

        window.Present();
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