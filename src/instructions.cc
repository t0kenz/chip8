#include <stdint.h>
#include <unordered_map>
#include "include/chip8.h"
#include "include/instructions.h"

const std::unordered_map<Mask, CallBack> instruction_table_0 = {
    {0x0, Chip8::cls},
    {0xE, Chip8::ret}
};

const std::unordered_map<Mask, CallBack> instruction_table_8 = {
    {0x0, Chip8::ld_vx_vy},
    {0x1, Chip8::or_vx_vy},
    {0x2, Chip8::and_vx_vy},
    {0x3, Chip8::xor_vx_vy},
    {0x4, Chip8::add_vx_vy},
    {0x5, Chip8::sub_vx_vy},
    {0x6, Chip8::shr_vx_vy},
    {0x7, Chip8::subn_vx_vy},
    {0xE, Chip8::shl_vx_vy}
};

const std::unordered_map<Mask, CallBack> instruction_table_e = {
    {0x9E, Chip8::skp_vx},
    {0xA1, Chip8::sknp_vx}
};

const std::unordered_map<Mask, CallBack> instruction_table_f = {
    {0x07, Chip8::ld_vx_dt},
    {0x0A, Chip8::ld_vx_key},
    {0x15, Chip8::ld_dt_vx},
    {0x18, Chip8::ld_st_vx},
    {0x1E, Chip8::add_i_vx},
    {0x29, Chip8::ld_f_vx},
    {0x33, Chip8::ld_b_vx},
    {0x55, Chip8::ld_i_vx},
    {0x65, Chip8::ld_vx_i}
};

const std::unordered_map<Mask, CallBack> instruction_table_rest = {
    {0x1, Chip8::jp_addr},
    {0x2, Chip8::call_addr},
    {0x3, Chip8::skip_next_ife_vxkk},
    {0x4, Chip8::skip_next_ifne_vxkk},
    {0x5, Chip8::skip_next_ife_vxvy},
    {0x6, Chip8::ld_vx_kk},
    {0x7, Chip8::add_vx_kk},
    {0x9, Chip8::skip_next_ifne_vx_vy},
    {0xA, Chip8::ld_iaddr},
    {0xB, Chip8::jp_offset},
    {0xC, Chip8::rnd_vx_kk},
    {0xD, Chip8::draw_vx_vy_nibble}
};

