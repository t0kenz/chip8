#ifndef CHIP_8_H
#define CHIP_8_h

#include <array>

// 16 general purpose 8-bit registers
// opcode 16 bit length, 8 bit for instruction and 8 for operand.
// Vf not used
// two timers, one for delay and one sound.
// program counter -- stores current executing address.
// stack pointer

class Chip8 {
  public:

    static constexpr std::array<uint8_t, 80> font = {
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F     
    };


    // program starts at address 0x200
    explicit Chip8() : program_counter(0x200)  {}

    bool fetch_instruction();
    uint8_t decode_instruction();
    uint8_t execute();
    void run_cycle();

    void load_font();

    void test() {
     /* std::cout << std::endl;
      for (int i = 0; i < memory.size(); i++) {
        if (memory[i] != 0)
        std::cout << memory[i];
      }
      */
    }

  private:
    static constexpr size_t memory_size = 4096;
    static constexpr size_t stack_depth = 12;
    static constexpr size_t display_size = 64 * 32;

    std::array<uint8_t, memory_size> memory;
    std::array<uint8_t, stack_depth> stack;
    std::array<uint8_t, display_size> display;

    uint16_t program_counter;
    uint8_t stack_pointer;

    uint8_t timer_delay;
    uint8_t sound_delay;

    std::array<uint8_t, 16> registers;
    
};
#endif
