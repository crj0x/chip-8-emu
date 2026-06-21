#pragma once

#include <SDL3/SDL.h>

class Platform
{
public:
    Platform();
    ~Platform();

private:
    SDL_Window* window{nullptr};

    uint32_t scale{16};
};
