#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "SDL2/SDL.h"
#include "nums.h"
#include <unordered_map>

struct Keyboard {
    const std::unordered_map<SDL_Keycode, u16> key_map {
      {SDLK_1, 0x1}, {SDLK_2, 0x2}, {SDLK_3, 0x3}, {SDLK_4, 0xC},
      {SDLK_q, 0x4}, {SDLK_w, 0x5}, {SDLK_e, 0x6}, {SDLK_r, 0xD},
      {SDLK_a, 0x7}, {SDLK_s, 0x8}, {SDLK_d, 0x9}, {SDLK_f, 0xE},
      {SDLK_z, 0xA}, {SDLK_x, 0x0}, {SDLK_c, 0xB}, {SDLK_v, 0xF}
    };

    u8 waiting_key = 0;
    std::array<bool, 16> keys{};
};

#endif
