#pragma once

#include <Engine/Engine.h>

struct Window {

    void Init();
    void Clear();
    void SetDrawGameplay();
    void SetDrawOther();
    void Present();
    void Shutdown();

    SDL_Texture* gameplay_texture;
    SDL_Texture* other_texture;
    SDL_Rect other_texture_rect;
    SDL_Window* window;
    SDL_Surface* icon;
};

// Global Window Data
struct GlobalWindowData {
    int w;
    int h;
    int scale;
    SDL_Renderer* rdr;
    SDL_Rect gameplay_viewport;
};
extern GlobalWindowData g_window;

#ifdef ENGINE_IMPLEMENTATION

void Window::Init() {
    window = SDL_CreateWindow("juice", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, g_window.w, g_window.h, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    SDL_UpdateWindowSurface(window);
    g_window.rdr = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    gameplay_texture = SDL_CreateTexture(g_window.rdr, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_TARGET, 768, 768);
    other_texture = SDL_CreateTexture(g_window.rdr, SDL_GetWindowPixelFormat(window), SDL_TEXTUREACCESS_TARGET, 768, 768);
    SDL_SetTextureBlendMode(other_texture, SDL_BLENDMODE_BLEND);

    icon = IMG_Load("assets/player/red.png");
    if (!icon) SDL_Log("icon.png not loaded");
    SDL_SetWindowIcon(window, icon);
}

void Window::Clear() {

    SDL_SetRenderDrawColor(g_window.rdr, 100, 100, 0, 0);
    SDL_RenderClear(g_window.rdr);

    // We don't have to clear gameplay_texture every pixel is drawn over every frame, so it's not necessarry.

    SDL_SetRenderTarget(g_window.rdr, other_texture);
    SDL_RenderClear(g_window.rdr);

    SDL_SetRenderDrawColor(g_window.rdr, 255, 255, 255, 255);
}

void Window::SetDrawGameplay() {
    SDL_SetRenderTarget(g_window.rdr, gameplay_texture);
}

void Window::SetDrawOther() {
    SDL_SetRenderTarget(g_window.rdr, other_texture);
}

void Window::Present() {
    SDL_SetRenderTarget(g_window.rdr, NULL);
    SDL_RenderCopy(g_window.rdr, gameplay_texture, & g_window.gameplay_viewport, NULL);
    other_texture_rect = {0, 0, g_window.w / g_window.scale, g_window.h / g_window.scale};
    SDL_RenderCopy(g_window.rdr, other_texture, & other_texture_rect, NULL);
    SDL_RenderPresent(g_window.rdr);
}


void Window::Shutdown() {
    SDL_Log("Shutting down window");
    SDL_FreeSurface(icon);
    SDL_DestroyRenderer(g_window.rdr);
    SDL_DestroyWindow(window);
}

GlobalWindowData g_window = {1800, 1000, 4, NULL, NULL};
Window window;

#endif