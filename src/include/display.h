#ifndef DISPLAY_H
#define DISPLAY_H

#include "SDL2/SDL.h"
#include "nums.h"
#include <array>
  
class Display {
  public:
    Display() {};
    // non-copyable
    Display(const Display&) = delete;
    Display& operator=(const Display&) = delete;

    ~Display() { 
      SDL_DestroyWindow(window);
      SDL_DestroyRenderer(renderer);
      SDL_DestroyTexture(texture);
      SDL_Quit(); 
    };

    bool initialize();
    void render_screen();

    std::array<u32, 2048> pixel_buf{};
  private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    
    static constexpr size_t display_width = 64;
    static constexpr size_t display_height = 32;
};

#endif
