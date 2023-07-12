#include <algorithm>
#include <fstream>
#include <iterator>
#include <string>
#include <cassert>

#include "include/chip8.h"
#include "include/keyboard.h"
//#define Debug

u16 Chip8::fetch_opcode() const {
    // merge the opcode by shifting the first byte and then ORing the second.
    // this follows from memory being stored as single bytes, therefore the instruction
    // is placed on 2 different spaces.
    u16 opcode = memory[program_counter] << 8 | memory[program_counter + 1];
    return opcode;
}

CallBack Chip8::fetch_instruction(const u16 instruction) {
    u8 most_significant_byte = instruction >> 12;
    u8 least_significant_byte = instruction & 0x000F;
    u8 least_two_significant_bytes = instruction & 0x00FF; 

    std::unordered_map<Mask, CallBack> lookup_table;
    // we want to parse which instruction to use, and since all
    // instructions are built up differently (most_significant_byte + lsb) and some are
    // (most_significant_byte + 0x00FF) or just most_significant_byte.
    // therefore we set instr_byte to which variant we want to look up.
    u8 instruction_byte = least_significant_byte;
    if (most_significant_byte == 0x0) {
        // lsb
        lookup_table = instruction_table_0;
    } else if (most_significant_byte == 0x8) {
        // lsb
        lookup_table = instruction_table_8;
    } else if (most_significant_byte == 0xE) { 
        instruction_byte = least_two_significant_bytes;
        lookup_table = instruction_table_e;
    } else if (most_significant_byte == 0xF) {
        instruction_byte = least_two_significant_bytes;
        lookup_table = instruction_table_f;
    } else {
        instruction_byte = most_significant_byte;
        lookup_table = instruction_table_rest;
    }

    try { 
        return lookup_table.at(instruction_byte);
    } catch(std::out_of_range& e) {
        std::cout << "Instruction that doesn't exist: " << std::hex << instruction << '\n';
    }
    return nullptr;
}

void Chip8::execute_instruction(const u16 instruction) {
    #ifdef Debug
        std::cout << "[Log]: Currently executing opcode: " << std::hex << instruction << '\n';
    #endif
    auto instruction_to_execute = fetch_instruction(instruction);    
    program_counter += 2;
    OpcodeFields opcode_fields(instruction);  
    instruction_to_execute(*this, opcode_fields);
}

void Chip8::load_font() {
    // According to the documentation, the convention is to put
    // all fonts in the memory region of 0x50-0x9F
    std::copy(font.begin(), font.end(), memory.begin() + 0x50);
}

void Chip8::run_cycle() {
    u16 opcode = fetch_opcode();
    execute_instruction(opcode);
}

void Chip8::load_program(const std::string& rom_path) {
    std::ifstream rom(rom_path, std::ios::binary);
    
    if (!rom.good())
        throw std::runtime_error("ROM couldn't be found\n");

    u32 program_start_offset = 0x200;
    while(rom.good()) {
        // this is "fine" range handling :) 
        memory[program_start_offset++ & 0xFFF] = rom.get();
    }
    load_font();
}

void Chip8::cls(Chip8& c8, const OpcodeFields& fields) {
    c8.display.fill({});
}

void Chip8::ret(Chip8& c8, const OpcodeFields& fields) {
    c8.program_counter = c8.stack[--c8.stack_pointer];
}

void Chip8::call_addr(Chip8& c8, const OpcodeFields& fields) {
    // Call subroutine at nnn 
    assert(c8.stack_pointer != 12);
    c8.stack[c8.stack_pointer++] = c8.program_counter;
    c8.program_counter = fields.nnn;
}

void Chip8::jp_addr(Chip8& c8, const OpcodeFields& fields) {
    c8.program_counter = fields.nnn;
}

void Chip8::skip_next_ife_vxkk(Chip8& c8, const OpcodeFields& fields) {
    auto& Vx = c8.registers[fields.x];
    if (Vx == fields.kk) {
        c8.program_counter += 2;
    }
}

void Chip8::skip_next_ifne_vxkk(Chip8& c8, const OpcodeFields& fields) {
    auto& Vx = c8.registers[fields.x];
    if (Vx != fields.kk) {
        c8.program_counter += 2;
    }
}

void Chip8::skip_next_ife_vxvy(Chip8& c8, const OpcodeFields& fields) {
    auto& Vx = c8.registers[fields.x];
    auto& Vy = c8.registers[fields.y];
    if (Vx == Vy) {
        c8.program_counter += 2;
    }
}

void Chip8::ld_vx_kk(Chip8& c8, const OpcodeFields& fields) {
    auto& Vx = c8.registers[fields.x];
    Vx = fields.kk;
}


void Chip8::add_vx_kk(Chip8& c8, const OpcodeFields& fields) {
    auto& Vx = c8.registers[fields.x];
    Vx += fields.kk;
}

void Chip8::ld_vx_vy(Chip8& c8, const OpcodeFields& fields) {
    // Vx = Vy 
    auto& Vx = c8.registers[fields.x];
    auto& Vy = c8.registers[fields.y];
    Vx = Vy;
}

void Chip8::or_vx_vy(Chip8& c8, const OpcodeFields& fields) {
    auto& Vx = c8.registers[fields.x];
    auto& Vy = c8.registers[fields.y];
    Vx |= Vy; 
}

void Chip8::and_vx_vy(Chip8& c8, const OpcodeFields& fields) {
    auto& Vx = c8.registers[fields.x];
    auto& Vy = c8.registers[fields.y];
    Vx &= Vy; 
}

void Chip8::xor_vx_vy(Chip8& c8, const OpcodeFields& fields) {
    auto& Vx = c8.registers[fields.x];
    auto& Vy = c8.registers[fields.y];
    Vx ^= Vy; 
}

void Chip8::add_vx_vy(Chip8& c8, const OpcodeFields& fields) {
    auto& Vx = c8.registers[fields.x];
    auto& Vy = c8.registers[fields.y];
    auto& Vf = c8.registers[15];

    auto res = Vx + Vy;
    // res can be deduced to u16, meaning the result will be greater,
    // and setting Vx to this result will result in Vx = res % 0xFF
    // in this case, let's just look at the 8 lowest bits of res.
    Vx = res & 0xFF;
    Vf = res > 0xFF ? 1 : 0;
}

void Chip8::sub_vx_vy(Chip8& c8, const OpcodeFields& fields) {
    auto& Vx = c8.registers[fields.x];
    auto Vy = c8.registers[fields.y];
    auto& Vf = c8.registers[15];

    // om x > y !overflow
    Vf = Vx > Vy;
    auto res = Vx - Vy;
    Vx = res & 0xFF;
}

void Chip8::shr_vx_vy(Chip8& c8, const OpcodeFields& fields) {
    auto& Vx = c8.registers[fields.x];
    auto Vy = c8.registers[fields.y];
    auto& Vf = c8.registers[15];

    // implementations seem to differ here, might cause bugs in some roms
    Vx = Vy >> 1;
    Vf = Vy & 0x1;
}

void Chip8::subn_vx_vy(Chip8& c8, const OpcodeFields& fields) {
    auto& Vx = c8.registers[fields.x];
    auto& Vy = c8.registers[fields.y];
    auto& Vf = c8.registers[15];

    auto res = Vy - Vx;
    Vx = res;
    Vf = Vy > Vx;
}

void Chip8::shl_vx_vy(Chip8& c8, const OpcodeFields& fields) {
    auto& Vx = c8.registers[fields.x];
    auto Vy = c8.registers[fields.y];
    auto& Vf = c8.registers[15];

    Vx = Vy << 1;
    Vf = (Vy >> 7) & 0x1;
}

void Chip8::skip_next_ifne_vx_vy(Chip8& c8, const OpcodeFields& fields) {
    auto& Vx = c8.registers[fields.x];
    auto& Vy = c8.registers[fields.y];

    if (Vx != Vy) {
        c8.program_counter += 2;
    }
}

void Chip8::ld_iaddr(Chip8& c8, const OpcodeFields& fields) {
    c8.index_register = fields.nnn;
}

void Chip8::jp_offset(Chip8& c8, const OpcodeFields& fields) {
    auto& V0 = c8.registers[0];
    c8.program_counter = fields.nnn + V0;
}

void Chip8::rnd_vx_kk(Chip8& c8, const OpcodeFields& fields) {
    auto& Vx = c8.registers[fields.x];
    Vx = std::uniform_int_distribution<>(0, 255)(c8.rnd) & fields.kk;
}

void Chip8::ld_vx_dt(Chip8& c8, const OpcodeFields& fields) {
    auto& Vx = c8.registers[fields.x];
    Vx = c8.timer_delay;
}

void Chip8::ld_vx_key(Chip8& c8, const OpcodeFields& fields) {
    // All execution stops until a key is pressed, then the value of that key is stored in Vx.
    // fields.x holds Vx
    // unsure if this works properly
    c8.keyboard.waiting_key = 0x80 | fields.x;
    c8.program_counter -= 2;
}

void Chip8::ld_dt_vx(Chip8& c8, const OpcodeFields& fields) {
    auto& Vx = c8.registers[fields.x];
    c8.timer_delay = Vx;
}

void Chip8::ld_st_vx(Chip8& c8, const OpcodeFields& fields) {
    auto& Vx = c8.registers[fields.x];
    c8.sound_delay = Vx;
}

void Chip8::add_i_vx(Chip8& c8, const OpcodeFields& fields) {
    auto& Vx = c8.registers[fields.x];
    auto& Vf = c8.registers[15];

    auto res = (c8.index_register & 0xFFF) + Vx;
    Vf = res >> 12;
    c8.index_register = res;
}

void Chip8::draw_vx_vy_nibble(Chip8& c8, const OpcodeFields& fields) {
    // "Wrap" starting position of the sprite
    u8 x = c8.registers[fields.x] % display_width;
    u8 y = c8.registers[fields.y] % display_height; 
    u8& Vf = c8.registers[15] = 0;

    // obtain the 'n'-nibble containing the sprite_height using kk and
    // shifting 4 times.
    const u8 sprite_height = fields.kk & 0xF;
    for (u8 n = 0; n < sprite_height; n++, y++) {
        const u8 sprite_data = c8.memory[(c8.index_register + n) & 0xFFF];
        // reset x_pixel_coord for each row
        u8 x_pixel_coord = x;
        for (i8 bit = 7; bit >= 0; bit--, x_pixel_coord++) {
            const u8 sprite_bit = (sprite_data >> bit) & 0x1;
            const u16 pixel_memory_address = x_pixel_coord + c8.display_width * y; 
             
            if (c8.display[pixel_memory_address & 0xFFF] == 1 &&
                sprite_bit == 1)
                Vf = 1;

            c8.display[pixel_memory_address & 0xFFF] ^= sprite_bit;

            if (x_pixel_coord + 1 >= display_width) break;
        }
        if (y + 1 >= display_height) break;
    }
}


void Chip8::skp_vx(Chip8& c8, const OpcodeFields& fields) {
    u8 vx = c8.registers[fields.x];
    if (c8.keyboard.keys[vx]) {
        c8.program_counter += 2;
    }
}

void Chip8::sknp_vx(Chip8& c8, const OpcodeFields& fields) {
    u8 vx = c8.registers[fields.x];
    if (!c8.keyboard.keys[vx]) {
        c8.program_counter += 2;
    }

}

void Chip8::ld_f_vx(Chip8& c8, const OpcodeFields& fields) {
    u8 digit = c8.registers[fields.x];
    c8.index_register = 0x50 + (5 * digit);
}

void Chip8::ld_b_vx(Chip8& c8, const OpcodeFields& fields) {
    u8 vx = c8.registers[fields.x];
    c8.memory[c8.index_register & 0xFFF] = vx / 100; // place the hundreds digit in memory at location in I
    c8.memory[(c8.index_register + 1) & 0xFFF] = (vx / 10) % 10; // place the tens digit in memory at location in I + 1
    c8.memory[(c8.index_register + 2) & 0xFFF] = vx % 10; // place the ones digit in memory at location in I + 2
}

void Chip8::ld_i_vx(Chip8& c8, const OpcodeFields& fields) {
    u8 vx = fields.x; 
    for (u8 reg = 0; reg <= vx; reg++) {
        c8.memory[c8.index_register++ & 0xFFF] = c8.registers[reg];  
    }
}

void Chip8::ld_vx_i(Chip8& c8, const OpcodeFields& fields) {
    u8 vx = fields.x;
    for (u8 reg = 0; reg <= vx; reg++) {
       c8.registers[reg] = c8.memory[c8.index_register++ & 0xFFF]; 
    }
}

void Chip8::test() {
    std::cout << '\n';
    for (auto& byte : memory) {
        std::cout << std::hex << static_cast<u16>(byte);
    }
    std::cout << '\n';
}

