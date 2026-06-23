#include "Chip8.hpp"
#include "Platform.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    Chip8 chip8(argv[1]);

    Platform platform;

    bool is_running{true};

    const uint64_t CYCLE_DELAY{SDL_NS_PER_SECOND / 700};
    constexpr uint64_t TIMER_DELAY{SDL_NS_PER_SECOND / 60};

    uint64_t cycle_accumulator{};
    uint64_t timer_accumulator{};

    uint64_t last_time = SDL_GetTicksNS();

    while (is_running)
    {
        uint64_t current_time = SDL_GetTicksNS();
        uint64_t delta_time = current_time - last_time;
        last_time = current_time;

        cycle_accumulator += delta_time;
        timer_accumulator += delta_time;

        // handle inputs
        is_running = platform.processInput(chip8.getKeys());

        // execute cycles
        while (cycle_accumulator >= CYCLE_DELAY)
        {
            cycle_accumulator -= CYCLE_DELAY;
            chip8.cycle();
        }

        // decrement timers
        while (timer_accumulator >= TIMER_DELAY)
        {
            timer_accumulator -= TIMER_DELAY;
            chip8.tickTimers();
        }

        platform.handleAudio(chip8.beepNeeded());

        platform.updateScreen(chip8.getDisplayState());
    }

    return 0;
}
