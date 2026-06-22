#include "Chip8.hpp"
#include "Platform.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    Chip8 chip8(argv[1]);

    Platform platform;

    bool is_running = true;

    while (is_running)
    {
        // handle inputs
        is_running = platform.processInput(chip8.getKeys());

        // emulate one CPU cycle
        chip8.cycle();

        // update window
        platform.updateScreen(chip8.getDisplayState());
    }

    return 0;
}
