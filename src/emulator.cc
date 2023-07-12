#include "include/emulator.h"

void Emulator::poll_events(SDL_Event& event, bool& interrupted) {
    while (SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_QUIT: interrupted = true; break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                if (auto search = keyboard.key_map.find(event.key.keysym.sym); search != keyboard.key_map.end()) {
                    // found key pressed in key_map, set it to pressed
                    keyboard.keys[search->second] = event.type == SDL_KEYDOWN;
                    if (event.type == SDL_KEYDOWN && (keyboard.waiting_key & 0x80)) {
                        // obtain vx
                        keyboard.waiting_key &= 0x7F; 
                        chip8.registers[keyboard.waiting_key] = search->second;
                        chip8.program_counter += 2;
                    }
                } else {
                    break;
                }
                break;
        }
    }
}

void Emulator::update_timers() {
    if (chip8.timer_delay > 0) {
        chip8.timer_delay--;
    }
}

void Emulator::update_screen(std::array<u8, 2048>& display_buf, std::array<u32, 2048>& rgb_buf) {
    for(u16 pixel_pos = 0; pixel_pos < 2048; pixel_pos++) {
        rgb_buf[pixel_pos] = 0xFFFFFF * display_buf[pixel_pos];
    }
}

// include amount of cycles so it is configurable somewhat
void Emulator::run(Display& display) {
    bool interrupted = false;
    SDL_Event event; 
    u32 cycles = 10;
    
    while (!interrupted) {
        for (u32 i = 0; i < cycles && !(keyboard.waiting_key & 0x80); i++) {
            chip8.run_cycle();  
        }

        update_screen(chip8.display, display.pixel_buf);
        display.render_screen();

        update_timers();

        poll_events(event, interrupted);
        SDL_Delay(1000/60);
    }
}
