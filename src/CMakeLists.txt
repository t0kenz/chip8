add_executable(chip8)

target_sources(chip8 
    PUBLIC
        chip8.cc 
        main.cc
        instructions.cc
        emulator.cc
        display.cc
        include/instructions.h
        include/chip8.h
        include/nums.h
        include/display.h
        include/emulator.h
)

target_include_directories(chip8 PUBLIC ${SDL2_INCLUDE_DIR})
target_link_libraries(chip8 ${SDL2_LIBRARY}) 

set_target_properties(chip8
    PROPERTIES
        CXX_STANDARD 20
        CXX_EXTENSIONS OFF
        CMAKE_CXX_STANDARD_REQUIRED ON
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)


