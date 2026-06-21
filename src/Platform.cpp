#include "Platform.hpp"

Platform::Platform()
{
    // initalise sdl
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    // create sdl window
    window = SDL_CreateWindow("Chip 8 Emulator", 64 * scale, 32 * scale, 0);
}

Platform::~Platform()
{
    // destroy sdl window
    SDL_DestroyWindow(window);

    // quit sdl
    SDL_Quit();
}
