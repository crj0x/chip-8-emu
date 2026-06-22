#include "Platform.hpp"

Platform::Platform()
{
    // initalise sdl
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    // create sdl window
    window = SDL_CreateWindow("Chip 8 Emulator", 64 * scale, 32 * scale, 0);

    // create renderer
    renderer = SDL_CreateRenderer(window, nullptr);

    // create texture
    // we use SDL_TEXTUREACCESS_STREAMING as this texture will change frequently
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);
}

Platform::~Platform()
{
    // destroy texture
    SDL_DestroyTexture(texture);

    // destroy renderer
    SDL_DestroyRenderer(renderer);

    // destroy sdl window
    SDL_DestroyWindow(window);

    // quit sdl
    SDL_Quit();
}

bool Platform::processInput(std::array<bool, 16>& keys)
{
    bool is_running = true;
    SDL_Event event;

    // handle quitting
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            is_running = false;
        }
    }

    const bool* key_states = SDL_GetKeyboardState(nullptr);
    // map the physical keyboard to chip-8 keys
    /*
        Original Keypad:    Modern Keyboard:
        1 2 3 C             1 2 3 4
        4 5 6 D             Q W E R
        7 8 9 E             A S D F
        A 0 B F             Z X C V
    */
    keys[0x1] = key_states[SDL_SCANCODE_1];
    keys[0x2] = key_states[SDL_SCANCODE_2];
    keys[0x3] = key_states[SDL_SCANCODE_3];
    keys[0xC] = key_states[SDL_SCANCODE_4];

    keys[0x4] = key_states[SDL_SCANCODE_Q];
    keys[0x5] = key_states[SDL_SCANCODE_W];
    keys[0x6] = key_states[SDL_SCANCODE_E];
    keys[0xD] = key_states[SDL_SCANCODE_R];

    keys[0x7] = key_states[SDL_SCANCODE_A];
    keys[0x8] = key_states[SDL_SCANCODE_S];
    keys[0x9] = key_states[SDL_SCANCODE_D];
    keys[0xE] = key_states[SDL_SCANCODE_F];

    keys[0xA] = key_states[SDL_SCANCODE_Z];
    keys[0x0] = key_states[SDL_SCANCODE_X];
    keys[0xB] = key_states[SDL_SCANCODE_C];
    keys[0xF] = key_states[SDL_SCANCODE_V];

    return is_running;
}

void Platform::updateScreen(const std::array<std::array<bool, 64>, 32>& display_state)
{
    std::array<uint32_t, 32 * 64> pixels;
    for (uint8_t y = 0; y < 32; ++y)
    {
        for (uint8_t x = 0; x < 64; ++x)
        {
            uint32_t color = (display_state[y][x]) ? 0xFFFFFFFF : 0x000000FF;
            pixels[64 * y + x] = color;
        }
    }

    SDL_UpdateTexture(texture, nullptr, pixels.data(), 64 * sizeof(uint32_t));

    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}
