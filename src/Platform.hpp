#pragma once

#include <SDL3/SDL.h>
#include <array>

class Platform
{
public:
    Platform();
    ~Platform();

    bool processInput(std::array<bool, 16>& keys);
    void updateScreen(const std::array<std::array<bool, 64>, 32>& display_state);

private:
    SDL_Window* window{};
    SDL_Renderer* renderer{};
    SDL_Texture* texture{};

    std::array<uint32_t, 32 * 64> pixels{};

    uint32_t scale{16};
};
