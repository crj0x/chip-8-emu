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

    // setup random number generation
    std::random_device rd; // obtain a random seed from hardware
    rand_gen.seed(rd());   // initialize the engine with seed
}

void Chip8::cycle()
{
    // fetch opcode
    opcode = (static_cast<uint16_t>(memory[pc]) << 8) | (static_cast<uint16_t>(memory[pc + 1]));
    pc += 2;

    // decode and execute
    decodeAndExecute();
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

uint8_t Chip8::getRandByte()
{
    uint8_t random_byte = distrib(rand_gen);
    return random_byte;
}

void Chip8::decodeAndExecute()
{
    uint16_t nnn = opcode & 0x0FFF;
    uint8_t kk = opcode & 0x00FF;
    uint8_t n = opcode & 0x000F;
    uint8_t x = (opcode & 0x0F00) >> 8;
    uint8_t y = (opcode & 0x00F0) >> 4;

    switch (opcode & 0xF000)
    {
    case 0x0000:
        switch (opcode & 0x0FFF)
        {
        // 00E0 - CLS (Clear the display)
        case 0x00E0:
            display = {};
            break;

        // 00EE - RET (Return from a subroutine)
        case 0x00EE:
            pc = stack[--sp];
            break;
        }
        break;

    // 1nnn - JP addr (Jump to location nnn)
    case 0x1000:
        pc = nnn;
        break;

    // 2nnn - CALL addr (Call subroutine at nnn)
    case 0x2000:
        stack[sp++] = pc;
        pc = nnn;
        break;

    // 3xkk - SE Vx, byte (Skips next instruction if Vx = kk)
    case 0x3000:
        if (V[x] == kk)
            pc += 2;
        break;

    // 4xkk - SNE Vx, byte (Skips next instruction if Vx != kk)
    case 0x4000:
        if (V[x] != kk)
            pc += 2;
        break;

    case 0x5000:
        switch (n)
        {
        // 5xy0 - SE Vx, Vy (Skips next instruction if Vx = Vy)
        case 0x00:
            if (V[x] == V[y])
                pc += 2;
            break;
        }
        break;

    // 6xkk - LD Vx, byte (Sets Vx = kk)
    case 0x6000:
        V[x] = kk;
        break;

    // 7xkk - ADD Vx, byte (Set Vx = Vx + kk)
    case 0x7000:
        V[x] += kk;
        break;

    case 0x8000:
        switch (n)
        {
        // 8xy0 - LD Vx, Vy (Set Vx = Vy)
        case 0x00:
            V[x] = V[y];
            break;

        // 8xy1 - OR Vx, Vy (Set Vx = Vx OR Vy)
        case 0x01:
            V[x] |= V[y];
            break;

        // 8xy2 - AND Vx, Vy (Set Vx = Vx AND Vy)
        case 0x02:
            V[x] &= V[y];
            break;

        // 8xy3 - XOR Vx, Vy (Set Vx = Vx XOR Vy)
        case 0x03:
            V[x] ^= V[y];
            break;

        // 8xy4 - ADD Vx, Vy (Set Vx = Vx + Vy, set VF = carry)
        case 0x04:
        {
            uint16_t sum = V[x] + V[y];
            uint8_t carry = (sum > 255 ? 1 : 0);
            V[x] = sum & 0x00FF;
            V[0xF] = carry;
            break;
        }

        // 8xy5 - SUB Vx, Vy (Set Vx = Vx - Vy, set VF = NOT borrow)
        case 0x05:
        {
            uint8_t not_borrow = (V[x] >= V[y] ? 1 : 0);
            V[x] -= V[y];
            V[0xF] = not_borrow;
            break;
        }

        // 8xy6 - SHR Vx {, Vy} (Set Vx = Vx SHR 1)
        case 0x06:
        {
            uint8_t lsb = V[x] & 0x01;
            V[x] >>= 1;
            V[0xF] = lsb;
            break;
        }

        // 8xy7 - SUBN Vx, Vy (Set Vx = Vy - Vx, set VF = NOT borrow)
        case 0x07:
        {
            uint8_t not_borrow = (V[y] >= V[x] ? 1 : 0);
            V[x] = V[y] - V[x];
            V[0xF] = not_borrow;
            break;
        }

        // 8xyE - SHL Vx {, Vy} (Set Vx = Vx SHL 1)
        case 0x0E:
        {
            uint8_t msb = (V[x] & 0x80) >> 7;
            V[x] <<= 1;
            V[0xF] = msb;
            break;
        }
        }
        break;

    case 0x9000:
        switch (n)
        {
        // 9xy0 - SNE Vx, Vy (Skip next instruction if Vx != Vy)
        case 0x00:
            if (V[x] != V[y])
                pc += 2;
            break;
        }
        break;

    // Annn - LD I, addr (Set I = nnn)
    case 0xA000:
        I = nnn;
        break;

    // Bnnn - JP V0, addr (Jump to location nnn + V0)
    case 0xB000:
        pc = nnn + V[0];
        break;

    // Cxkk - RND Vx, byte (Set Vx = random byte AND kk)
    case 0xC000:
        V[x] = getRandByte() & kk;
        break;

    // Dxyn - DRW Vx, Vy, nibble (Display n pixel tall sprite starting at memory location I at (Vx, Vy), set VF = collision)
    case 0xD000:
    {
        // note: collision is said to occur if any "on" pixel turns "off" as a result of the operation

        const uint8_t start_i = V[y] % 32;
        const uint8_t start_j = V[x] % 64;

        V[0xF] = 0;
        uint16_t addr = I;

        for (uint8_t i = 0; i < n; i++)
        {
            if (start_i + i >= 32)
                break;
            for (uint8_t j = 0; j < 8; j++)
            {
                if (start_j + j >= 64)
                    break;
                uint8_t sprite_pixel = memory[addr + i] & (0x80 >> j);
                if (sprite_pixel)
                {
                    // set flag if collision occurred
                    if (display[start_i + i][start_j + j])
                        V[0xf] = 1;
                    display[start_i + i][start_j + j] ^= 1;
                }
            }
        }
        break;
    }

    case 0xE000:
        switch (kk)
        {
        // Ex9E - SKP Vx (Skip next instruction if key with the value of Vx is pressed)
        case 0x9E:
            if (keys[V[x]])
                pc += 2;
            break;

        // ExA1 - SKNP Vx (Skip next instruction if key with the value of Vx is not pressed)
        case 0xA1:
            if (!keys[V[x]])
                pc += 2;
            break;
        }
        break;
    case 0xF000:
        switch (kk)
        {
        // Fx07 - LD Vx, DT (Set Vx = delay timer value)
        case 0x07:
            V[x] = delay_timer;
            break;

        // Fx0A - LD Vx, K (Wait for a key press, store the value of the key in Vx)
        case 0x0A:
        {
            bool key_detected = false;

            for (uint8_t i = 0; i < 16; ++i)
            {
                if (keys[i])
                {
                    V[x] = i;
                    key_detected = true;
                    break;
                }
            }

            // if no key is pressed we undo the increment in pc
            // this means that in the next cycle also the same instruction will run
            if (!key_detected)
                pc -= 2;
            break;
        }

        // Fx15 - LD DT, Vx (Set delay timer = Vx)
        case 0x15:
            delay_timer = V[x];
            break;

        // Fx18 - LD ST, Vx (Set sound timer = Vx)
        case 0x18:
            sound_timer = V[x];
            break;

        // Fx1E - ADD I, Vx (Set I = I + Vx)
        case 0x1E:
            I += V[x];
            break;

        // Fx29 - LD F, Vx (Set I = location of sprite for digit Vx)
        case 0x29:
            I = FONT_START + V[x] * 5;
            break;

        // Fx33 - LD B, Vx (Store BCD representation of Vx in memory locations I, I+1, and I+2)
        case 0x33:
            memory[I + 2] = V[x] % 10;
            memory[I + 1] = (V[x] / 10) % 10;
            memory[I] = V[x] / 100;
            break;

        // Fx55 - LD [I], Vx (Store registers V0 through Vx in memory starting at location I)
        case 0x55:
        {
            uint16_t addr = I;
            for (uint8_t i = 0; i <= x; i++)
            {
                memory[addr + i] = V[i];
            }
            break;
        }

        // Fx65 - LD Vx, [I] (Read registers V0 through Vx from memory starting at location I)
        case 0x65:
        {
            uint16_t addr = I;
            for (uint8_t i = 0; i <= x; i++)
            {
                V[i] = memory[addr + i];
            }
            break;
        }
        }
        break;
    }
}
