#pragma once

#include <array>
#include <cstdint>
#include <random>
#include <string>

class Chip8
{
public:
    Chip8(const std::string& rom_path);
    bool loadRom(const std::string& rom_path);
    void loadFont();
    void decodeAndExecute();
    void cycle();
    const std::array<std::array<bool, 64>, 32>& getDisplayState();
    std::array<bool, 16>& getKeys();
    uint8_t getRandByte();

private:
    std::array<uint8_t, 4096> memory{}; // 4 kilobytes of ram
    uint16_t pc{};                      // instruction pointer / program counter
    uint16_t I{};                       // special register (primarily used to point at memory locations)
    std::array<uint16_t, 16> stack{};   // to call subroutines / functions
    uint8_t sp{};                       // stack pointer
    std::array<uint8_t, 16> V{};        // 16 general purpose registers (V0 - VF)
    uint8_t delay_timer{};
    uint8_t sound_timer{};
    std::array<std::array<bool, 64>, 32> display_state{};
    uint16_t opcode{};
    std::array<bool, 16> keys{}; // true if pressed, false if not pressed

    static constexpr uint16_t START_ADDRESS{0x200}; // address for loading rom
    static constexpr uint16_t FONT_START{0x50};     // address for loading font

    // random number generation
    // the engine is a member variable as it needs to maintain its state
    std::mt19937 rand_gen;
    std::uniform_int_distribution<uint8_t> distrib{0, 255};
};
