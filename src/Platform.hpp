#pragma once

#include <SDL3/SDL.h>

class Platform
{
public:
    Platform();
    ~Platform();

private:
    SDL_Window* window{};
    SDL_Renderer* renderer{};
    SDL_Texture* texture{};

    uint32_t scale{16};
};
