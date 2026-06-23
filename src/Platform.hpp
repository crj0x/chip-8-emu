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
    void handleAudio(bool beep_needed);

private:
    SDL_Window* window{};
    SDL_Renderer* renderer{};
    SDL_Texture* texture{};

    // beep sound properties
    // number of audio data points processed per second
    static constexpr int AUDIO_SAMPLE_RATE{44100};
    // freq of beep tone in Hz
    static constexpr float BEEP_FREQ{440.0f};
    // this is how much beep phase changes between each audio data point
    static constexpr float beep_phase_increment{2.0f * SDL_PI_F * (BEEP_FREQ / AUDIO_SAMPLE_RATE)};
    // volume to control amplitude of beep wave
    static constexpr float volume{0.5f};
    // keeps track of current phase of beep cycle
    float beep_phase{};
    // buffer to hold generated audio data points
    std::array<float, 1024> audio_buffer;

    SDL_AudioStream* audio_stream{};
    SDL_AudioSpec spec{SDL_AUDIO_F32, 1, AUDIO_SAMPLE_RATE};

    std::array<uint32_t, 32 * 64> pixels{};

    uint32_t scale{16};
};
