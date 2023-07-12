#ifndef EMULATOR_H
#define EMULATOR_H
#include "SDL2/SDL.h"
#include "chip8.h"
#include <array>
#include "display.h"
#include "keyboard.h"

class Emulator {
  public:  
    Emulator(const Emulator&) = delete;
    Emulator& operator=(const Emulator&) = delete;

    explicit Emulator(Chip8& c8, Keyboard& keyboard) : keyboard(keyboard), chip8(c8){};
        
    void run(Display& display);
  private:
    void poll_events(SDL_Event& event, bool& interrupted);
    void update_timers();  
    void update_screen(std::array<u8, 2048>& display_buf, std::array<u32, 2048>& rgb_buf);
    
    Keyboard& keyboard;
    Chip8& chip8; 
};

#endif
