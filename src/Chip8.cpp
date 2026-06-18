#include "Chip8.hpp"
#include <fstream>
#include <iostream>

constexpr std::array<std::array<uint8_t, 5>, 16> FONT_SET{{
    {0xF0, 0x90, 0x90, 0x90, 0xF0}, // 0
    {0x20, 0x60, 0x20, 0x20, 0x70}, // 1
    {0xF0, 0x10, 0xF0, 0x80, 0xF0}, // 2
    {0xF0, 0x10, 0xF0, 0x10, 0xF0}, // 3
    {0x90, 0x90, 0xF0, 0x10, 0x10}, // 4
    {0xF0, 0x80, 0xF0, 0x10, 0xF0}, // 5
    {0xF0, 0x80, 0xF0, 0x90, 0xF0}, // 6
    {0xF0, 0x10, 0x20, 0x40, 0x40}, // 7
    {0xF0, 0x90, 0xF0, 0x90, 0xF0}, // 8
    {0xF0, 0x90, 0xF0, 0x10, 0xF0}, // 9
    {0xF0, 0x90, 0xF0, 0x90, 0x90}, // A
    {0xE0, 0x90, 0xE0, 0x90, 0xE0}, // B
    {0xF0, 0x80, 0x80, 0x80, 0xF0}, // C
    {0xE0, 0x90, 0x90, 0x90, 0xE0}, // D
    {0xF0, 0x80, 0xF0, 0x80, 0xF0}, // E
    {0xF0, 0x80, 0xF0, 0x80, 0x80}  // F
}};

Chip8::Chip8(const std::string& rom_path)
{
    loadFont();
    loadRom(rom_path);
}

bool Chip8::loadRom(const std::string& rom_path)
{
    // open as binary (std::ios::binary) and immediately seek to end (std::ios::ate)
    std::ifstream rom(rom_path, std::ios::binary | std::ios::ate);

    if (!rom)
    {
        return false;
    }

    // get the current position (which is the file size)
    std::streamsize rom_size = rom.tellg();
    // seek back to beginning of the file
    rom.seekg(0, std::ios::beg);

    if (!rom.read(reinterpret_cast<char*>(&memory[Chip8::START_ADDRESS]), rom_size))
    {
        return false;
    }

    return true;
}

void Chip8::loadFont()
{
    // although the font can be stored anywhere in the first 512 bytes (0x000 - 0x1FF),
    // it is popular to put it at 0x50 - 0x9F, that convention has been followed here.

    for (size_t i = 0; i < FONT_SET.size(); i++)
    {
        for (size_t j = 0; j < FONT_SET[i].size(); j++)
        {
            memory[Chip8::FONT_START + i * 5 + j] = FONT_SET[i][j];
        }
    }
}
