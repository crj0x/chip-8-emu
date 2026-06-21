#include "Chip8.hpp"
#include "Platform.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    Chip8 chip8(argv[1]);

    Platform platform;

    SDL_Delay(3000);

    return 0;
}
