#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

#include <algorithm>
#include <cstddef>
#include <iostream>

#include "include/nums.h"
#include "include/chip8.h"
#include "include/display.h"
#include "include/keyboard.h"
#include "include/emulator.h"

int main(int argc, char** argv) {
    if (argc < 2 || argc > 2) {
        std::cout << "Too few arguments passed.\n";
        std::cout << "Example usage: \n";
        std::cout << "./chip8 <file_path_here>\n";
        std::cout << "./chip8 my_dir/my_chip8_rom.ch8\n";

        return 0;
    }

    Keyboard keyboard;
    Chip8 chip8(keyboard);

    chip8.load_program(std::string(argv[1]));
    
    Display display;
    if (!display.initialize()) {
        std::cerr << "Display couldn't be initialized\n";
        return 1;
    }

    Emulator emulator(chip8, keyboard);
    emulator.run(display);
}

