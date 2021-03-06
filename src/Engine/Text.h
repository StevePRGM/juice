#pragma once

#include <string>

#include "Window.h"

struct Text {
    void LoadFont(std::string path = "assets/font.ttf");
    void DestroyFont();
    SDL_Texture* CreateTexture(std::string text);
    TTF_Font* main_font = NULL;
    SDL_Color font_color = {255, 255, 255};
};
extern Text g_text;

#ifdef ENGINE_IMPLEMENTATION

void Text::LoadFont(std::string path) {
    main_font = TTF_OpenFont(path.c_str(), 10);
    if (!main_font) SDL_Log("can't load font"); else SDL_Log("Font loaded.");
}

void Text::DestroyFont() {
    TTF_CloseFont(main_font);
}

SDL_Texture* Text::CreateTexture(std::string text) {
    SDL_Surface* temp_surface = TTF_RenderText_Solid(main_font, text.c_str(), font_color);
    if (!temp_surface) SDL_Log("failed to create surface\n");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(g_window.rdr, temp_surface);

    SDL_FreeSurface(temp_surface);

    return texture;
}

Text g_text;

#endif