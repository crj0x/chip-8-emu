#include "Chip8.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "CHIP-8 Emulator\n";
    Chip8 chip8(argv[1]);

    return 0;
}
