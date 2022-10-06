
#ifndef CHIP8EMULATOR_CPU_H
#define CHIP8EMULATOR_CPU_H

#include <array>
#include <cstdlib>
#include <ctime>

using Byte = uint8_t;           //1 byte
using Word = uint16_t;          //2 byte

enum KeyBoard{
    KEY_X = 0,
    KEY_1 = 1,
    KEY_2 = 2,
    KEY_3 = 3,
    KEY_Q = 4,
    KEY_W = 5,
    KEY_E = 6,
    KEY_A = 7,
    KEY_S = 8,
    KEY_D = 9,
    KEY_Z = 10, //A
    KEY_C = 11, //B
    KEY_4 = 12, //C
    KEY_R = 13, //D
    KEY_F = 14, //E
    KEY_V = 15, //F
    MAX_KEYPAD_SIZE
};

/*
 * NNN  - refers to hex address
 * NN   - hex byte
 * N    -
 */

class Cpu {
public:
    using CpuFunc = void (Cpu::*)();    //pointer to member function
    //initialize
    void Init(Byte *memPtr, Byte *vidBufPtr, Byte* keyPtr);

    //execute CPU cycle
    void ExecuteCPU();

private:
    //set up member function pointer
    void FuncPointerSetUp();

    //invalid instruction
    void OPcode_NULL(){}

    //execute opcode on table
    void Table00();
    void Table8();
    void TableE();
    void TableF();

    //clear the screen
    void OPcode_00E0();

    //return from a subroutine
    void OPcode_00EE();

    //jump to address NNN
    void OPcode_1NNN();

    //Execute subroutine starting at address NNN
    void OPcode_2NNN();

    //skip following instruction if value of register VX equals NN
    void OPcode_3XNN();

    //Skip the following instruction if the value of register VX is not equal to NN
    void OPcode_4XNN();

    //Skip the following instruction if the value of register VX is equal to the value of register VY
    void OPcode_5XY0();

    //Store number NN in register VX
    void OPcode_6XNN();

    //Add the value NN to register VX
    void OPcode_7XNN();

    //Store the value of register VY in register VX
    void OPcode_8XY0();

    //Set VX to VX OR VY
    void OPcode_8XY1();

    //Set VX to VX AND VY
    void OPcode_8XY2();

    //Set VX to VX XOR VY
    void OPcode_8XY3();

    //Add the value of register VY to register VX
    // borrow == true ? VF = 0x00 : VF = 0x01;
    void OPcode_8XY4();

    //Subtract the value of register VY from register VX
    // borrow == true ? VF = 0x00 : VF = 0x01;
    void OPcode_8XY5();

    //Store the value of register VY shifted right one bit in register VX
    //Set register VF to the least significant bit prior to the shift- VY is unchanged
    void OPcode_8XY6();

    //Set register VX to the value of VY minus VX
    // borrow == true ? VF = 0x00 : VF = 0x01;
    void OPcode_8XY7();

    //Store the value of register VY shifted left one bit in register VX
    //Set register VF to the most significant bit prior to the shift- VY is unchanged
    void OPcode_8XYE();

    //Skip the following instruction if the value of register VX is not equal to the value of register VY
    void OPcode_9XY0();

    //Store memory address NNN in register I
    void OPcode_ANNN();

    //Jump to address NNN + V0
    void OPcode_BNNN();

    //Set VX to a random number with a mask of NN
    void OPcode_CXNN();

    //Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I
    // any pixels changed to unset ? VF = 0x01 : VF = 0x00;
    void OPcode_DXYN();

    //Skip the following instruction if the key corresponding to the hex value currently stored in register VX is pressed
    void OPcode_EX9E();

    //Skip the following instruction if the key corresponding to the hex value currently stored in register VX is not pressed
    void OPcode_EXA1();

    //Store the current value of the delay timer in register VX
    void OPcode_FX07();

    //Wait for a keypress and store the result in register VX
    void OPcode_FX0A();

    //Set the delay timer to the value of register VX
    void OPcode_FX15();

    //Set the sound timer to the value of register VX
    void OPcode_FX18();

    //Add the value stored in register VX to register I
    void OPcode_FX1E();

    //Set I to the memory address of the sprite data corresponding to the hexadecimal digit stored in register VX
    void OPcode_FX29();

    //Store the binary-coded decimal equivalent of the value stored in register VX at addresses I, I + 1, and I + 2
    void OPcode_FX33();

    //tore the values of registers V0 to VX inclusive in memory starting at address I
    //I is set to I + X + 1 after operation
    void OPcode_FX55();

    //Fill registers V0 to VX inclusive with the values stored in memory starting at address I
    //I is set to I + X + 1 after operation
    void OPcode_FX65();

private:
    //Function Pointer Table
    CpuFunc     tableF[0xFF+1]{};
    CpuFunc     table[0xF + 1]{};
    CpuFunc     table0[0xF + 1]{};
    CpuFunc     table8[0xF + 1]{};
    CpuFunc     tableE[0xF + 1]{};
    //cpu "stack"
    Word        stack[16]{};
    //registers
    Byte        registers[16]{};
    //pointer to main memory
    Byte*       mainMemoryPtr{nullptr};
    Byte*       videoBufferPtr{nullptr};
    Byte*       keyPadPtr{nullptr};
    //index register is uses to store memory addresses for use in operations
    Word        indexRegister{0};
    //holds the address of the next instruction to execute; pc starts at 0x200
    Word        programCounter{0x200};
    //index to the stack
    Word        stackIndex{0};
    //Current OPCODE
    Word        opcode{0};
    //decrement at the rate of 60Mhz
    Byte        delayTimer{0};
    //(decrement if non-zero) a single tone will buzz when its non-zero
    Byte        soundTimer{0};
};


#endif //CHIP8EMULATOR_CPU_H
