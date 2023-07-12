#ifndef CHIP_8_H
#define CHIP_8_H

#include <array>
#include <iostream>
#include <string>
#include <unordered_map>
#include <random>
#include "nums.h"

struct OpcodeFields;
class Keyboard;
class Chip8;

using CallBack = void(*)(Chip8&, const OpcodeFields& fields);
using Mask = u16;

class Chip8 {
  public:
    static constexpr size_t display_width = 64;
    static constexpr size_t display_height = 32;
    static constexpr size_t display_size = display_width * display_height;

    std::array<u8, display_size> display{};

    static constexpr std::array<u8, 80> font = {
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
    explicit Chip8(Keyboard& keyboard) : program_counter(0x200), keyboard(keyboard) {}
    
    CallBack fetch_instruction(const u16 instruction);
    u16 fetch_opcode() const; 
    void execute_instruction(const u16 instruction);
    void run_cycle();

    void load_font();
    void load_program(const std::string& rom_path);
    void test();

    static void cls(Chip8& c8, const OpcodeFields& fields);
    static void ret(Chip8& c8, const OpcodeFields& fields);
    static void jp_addr(Chip8& c8, const OpcodeFields& fields);
    static void call_addr(Chip8& c8, const OpcodeFields& fields);
    static void skip_next_ife_vxkk(Chip8& c8, const OpcodeFields& fields);
    static void skip_next_ifne_vxkk(Chip8& c8, const OpcodeFields& fields);
    static void skip_next_ife_vxvy(Chip8& c8, const OpcodeFields& fields);
    static void ld_vx_kk(Chip8& c8, const OpcodeFields& fields);
    static void add_vx_kk(Chip8& c8, const OpcodeFields& fields);
    static void ld_vx_vy(Chip8& c8, const OpcodeFields& fields);
    static void or_vx_vy(Chip8& c8, const OpcodeFields& fields);
    static void and_vx_vy(Chip8& c8, const OpcodeFields& fields);
    static void xor_vx_vy(Chip8& c8, const OpcodeFields& fields);
    static void add_vx_vy(Chip8& c8, const OpcodeFields& fields);
    static void sub_vx_vy(Chip8& c8, const OpcodeFields& fields);
    static void shr_vx_vy(Chip8& c8, const OpcodeFields& fields);
    static void subn_vx_vy(Chip8& c8, const OpcodeFields& fields);
    static void shl_vx_vy(Chip8& c8, const OpcodeFields& fields);
    static void skip_next_ifne_vx_vy(Chip8& c8, const OpcodeFields& fields);
    static void ld_iaddr(Chip8& c8, const OpcodeFields& fields);
    static void jp_offset(Chip8& c8, const OpcodeFields& fields);
    static void rnd_vx_kk(Chip8& c8, const OpcodeFields& fields);
    static void draw_vx_vy_nibble(Chip8& c8, const OpcodeFields& fields);
    static void skp_vx(Chip8& c8, const OpcodeFields& fields);
    static void sknp_vx(Chip8& c8, const OpcodeFields& fields);
    static void ld_vx_dt(Chip8& c8, const OpcodeFields& fields);
    static void ld_vx_key(Chip8& c8, const OpcodeFields& fields);
    static void ld_dt_vx(Chip8& c8, const OpcodeFields& fields);
    static void ld_st_vx(Chip8& c8, const OpcodeFields& fields);
    static void add_i_vx(Chip8& c8, const OpcodeFields& fields);
    static void ld_f_vx(Chip8& c8, const OpcodeFields& fields);
    static void ld_b_vx(Chip8& c8, const OpcodeFields& fields);
    static void ld_i_vx(Chip8& c8, const OpcodeFields& fields);
    static void ld_vx_i(Chip8& c8, const OpcodeFields& fields);

  private:
    static constexpr size_t memory_size = 4096;
    static constexpr size_t stack_depth = 12;
    
    std::array<u8, memory_size> memory{};
    std::array<u16, stack_depth> stack{};

    std::array<u8, 16> registers{};

    u16 program_counter;
    u16 index_register;
    u8 stack_pointer = 0;

    u8 sound_delay;
    u8 timer_delay;

    std::mt19937 rnd{};
    
    friend class Emulator;
    Keyboard& keyboard;
};

struct OpcodeFields {
    // x refers to which x register
    u8 x;
    // y refers to which y register
    u8 y;
    // nnn is the address
    u16 nnn;
    // 
    u8 kk;

    OpcodeFields(u16 opcode) : x((opcode >> 8) & 0xF), 
                                    y ((opcode >> 4) & 0xF), 
                                    nnn(opcode & 0xFFF),
                                    kk(opcode & 0xFF) {}
};

extern const std::unordered_map<Mask, CallBack> instruction_table_0; 
extern const std::unordered_map<Mask, CallBack> instruction_table_8; 
extern const std::unordered_map<Mask, CallBack> instruction_table_e; 
extern const std::unordered_map<Mask, CallBack> instruction_table_f; 
extern const std::unordered_map<Mask, CallBack> instruction_table_rest; 

#endif
