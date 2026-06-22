#pragma once

#include <SDL3/SDL.h>
#include <array>

class Platform
{
public:
    Platform();
    ~Platform();

    bool processInput(std::array<bool, 16>& keys);

private:
    SDL_Window* window{};
    SDL_Renderer* renderer{};
    SDL_Texture* texture{};

    uint32_t scale{16};
};
