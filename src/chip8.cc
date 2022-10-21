#include "chip8.h"
#include <cstring>
#include <algorithm>

uint16_t Chip8::fetch_opcode() {
    // merge the opcode by shifting the first byte and then ORing the second.
    // this follows from memory being stored as single bytes, therefore the instruction
    // is placed on 2 different spaces.
    uint16_t opcode = memory[program_counter] << 8 | memory[program_counter + 1];
    return opcode;


}

void Chip8::execute_instruction(const uint16_t instruction) {
   // Potential issues: instruction might be 0, meaning we have reached the end or faulty program. 
   //
    switch(instruction) {
        

    }
}

void Chip8::run_cycle() {
    uint16_t opcode = fetch_opcode();
    execute_instruction(opcode);
}


// According to the documentation, the convention is to put
// all fonts in the memory region of 0x50-0x9F
void Chip8::load_font() {
    std::copy(font.begin(), font.end(), memory.begin());
}


