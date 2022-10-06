
#ifndef CHIP8EMULATOR_EMULATOR_H
#define CHIP8EMULATOR_EMULATOR_H

#include <filesystem>
#include <vector>
#include <iostream>
#include "renderer.h"
#include "cpu.h"

//emulator "container"
//handles cpu emulation
//handles interrupts
//loads rom into main memory

class Emulator {
public:
    //runs the emulator loop
    void Run();
    //initialize sub devices
    void Init();
    //loads rom into CPU memory address space 0x200 - 0xFFF
    void LoadRom(const char *filename);

    static void updateDraw(){updateNewDrawFlag = true;}

private:
    //update keymap from user input
    void PollInput();

private:
    //memory buffer; address space ranges from 0x000 to 0xFFF
    Byte            mainMemory[4096]{};
    //memory buffer used to store graphics for display
    Byte            videoBuffer[64*32]{};
    //KeyMap
    Byte            keyMap[MAX_KEYPAD_SIZE]{};
    //window glfw context
    Window          window;
    //cpu context
    Cpu             cpu;
    //Renderer
    Renderer        renderer;
    static inline bool updateNewDrawFlag = true;
};


#endif //CHIP8EMULATOR_EMULATOR_H
