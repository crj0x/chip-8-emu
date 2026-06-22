#include "Platform.hpp"

Platform::Platform()
{
    // initalise sdl
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    // create sdl window
    window = SDL_CreateWindow("Chip 8 Emulator", 64 * scale, 32 * scale, 0);

    // create renderer
    renderer = SDL_CreateRenderer(window, nullptr);

    // create texture
    // we use SDL_TEXTUREACCESS_STREAMING as this texture will change frequently
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
}

Platform::~Platform()
{
    // destroy texture
    SDL_DestroyTexture(texture);

    // destroy renderer
    SDL_DestroyRenderer(renderer);

    // destroy sdl window
    SDL_DestroyWindow(window);

    // quit sdl
    SDL_Quit();
}
