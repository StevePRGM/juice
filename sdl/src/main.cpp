#pragma once

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "Window.hpp"
#include "GlobalWindowData.hpp"
#include "Text.hpp"
#include "Clock.cpp"

#include "Player.hpp"
#include "Map.hpp"

GlobalWindowData global_window_data = {640, 640, 4, NULL, SDL_GetKeyboardState(NULL)};

int main(int argc, char* argv[]) {
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Text::LoadFont();
    srand(time(0));

    Window window;
    Clock clock;
    Player player;
    Map map;

    player.LoadTexture();
    player.GiveDT(& clock.dt);

    map.LoadTexture();
    map.CreateMapTexture();

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
                    if (event.key.keysym.sym == SDLK_ESCAPE)
                        quit = true;
                    break;
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

        player.Update();


        // DRAW

        window.Clear();

        map.Draw();
        player.Draw();

        //SDL_Log("%i", global_window_data.w);

        SDL_RenderPresent(global_window_data.rdr);
    }

    player.DestroyTexture();
    map.DestroyTextures();

    Text::DestroyFont();
    TTF_Quit();
        IMG_Quit();
        window.Shutdown();
    SDL_Quit();
    return 0;
}