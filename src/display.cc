#include "include/display.h"
#include <iostream>

bool Display::initialize() {
    if((SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) == -1)) { 
        std::cerr << "Could not initialize SDL " << SDL_GetError() <<
            '\n';
        return false;
    }

    window = SDL_CreateWindow("Chip8",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              display_width * 4,
                              display_height * 6,
                              SDL_WINDOW_RESIZABLE);

    if (!window) {    
        std::cerr << "Could not create window " << SDL_GetError() << '\n';
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cerr << "Could not create renderer " << SDL_GetError() <<
            '\n';
        return false;
    }

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, display_width, display_height);
    if (!texture) {
        std::cerr << "Could not create texture " << SDL_GetError() <<
        '\n';
        return false;
    }
       
    return true; 
}

void Display::render_screen() {
    SDL_UpdateTexture(texture, nullptr, pixel_buf.data(),
                      // pitch: number of bytes per row = 32 entries
                      // per row * 4 bytes (int)
                      display_width * 4);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

