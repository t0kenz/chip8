## Chip8 Emulator
"Works" for a majority of the typical benchmark ROMs. Probably has issues in certain cases, such as instructions that rely on timing etc.

### Dependencies
Requires SDL2

### Building
    git clone https://github.com/t0kenz/chip8
    inside of chip8 root dir
    $ mkdir build && cd $_
    $ cmake .. && make

### Example usage 
    ./chip8 my_dir/my_chip8_rom.ch8   

### TODO
Lots of todos (trust me)
the current structure is ~~, and I'd like to rewrite certain parts of the code (instructions, scalability, modularization, etc.)
