#pragma once

#include <array>
#include <cstdint>
#include <string>

class Chip8
{
public:
    bool loadRom(const std::string& rom_path);
    void loadFont();

    void cycle();

private:
    std::array<uint8_t, 4096> memory{}; // 4 kilobytes of ram
    uint16_t pc{};                      // instruction pointer / program counter
    uint16_t I{};                       // special register (primarily used to point at memory locations)
    std::array<uint16_t, 16> stack{};   // to call subroutines / functions
    uint16_t sp{};                      // stack pointer
    std::array<uint8_t, 16> V{};        // 16 general purpose registers (V0 - VF)
    uint8_t delay_timer{};
    uint8_t sound_timer{};
    std::array<std::array<uint8_t, 64>, 32> display{};
    uint16_t opcode{};

    static constexpr uint16_t START_ADDRESS{0x200}; // address for loading rom
    static constexpr uint16_t FONT_START{0x50};     // address for loading font
};
