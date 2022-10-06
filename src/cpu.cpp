
#include "cpu.h"
#include "emulator.h"

Byte fontData[80] ={
        0xF0, 0x90, 0x90, 0x90, 0xF0,       // 0
        0x20, 0x60, 0x20, 0x20, 0x70,       // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0,   // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0,   // 3
        0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
        0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
        0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
        0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
        0xF0, 0x80, 0xF0, 0x80, 0x80   // F
};

void Cpu::Init(Byte *memPtr, Byte *vidBufPtr, Byte* keyPtr) {
    //set ptr to main memory & video memory
    mainMemoryPtr = memPtr;
    videoBufferPtr = vidBufPtr;
    keyPadPtr = keyPtr;

    //memset register and stack to 0
    memset(registers, 0, 16);
    memset(stack, 0, 16);

    //pointer function table setup
    for(int index = 0; index <= 0xF; ++index){
        table[index] = &Cpu::OPcode_NULL;
        table0[index] = &Cpu::OPcode_NULL;
        table8[index] = &Cpu::OPcode_NULL;
        tableE[index] = &Cpu::OPcode_NULL;
    }

    for(int index = 0; index <= 0xFFu; ++index){
        tableF[index] = &Cpu::OPcode_NULL;
    }

    //TODO: LOAD FONT MEMORY
    for(int i = 0; i < 80; i++){
        memPtr[0x50 + i] = fontData[i];
    }


    FuncPointerSetUp();

    //initialize random time
    std::srand(212121);
}

void Cpu::ExecuteCPU() {
    //fetch OPCODE and incremente the program counter
    opcode = (mainMemoryPtr[programCounter] << 8u) | mainMemoryPtr[programCounter+1];
    std::cout << "OPCODE EXECUTING: " << std::hex << opcode << "\n";
    std::cout << std::hex << "Program counter: " << programCounter << "\n";
    programCounter +=2;

    //decode and execute
    ( this->*(table[ ((opcode & 0xF000u) >> 12u)]) )();

    //decrease the delay timer if set
    if(delayTimer > 0)
        --delayTimer;

    std::cout << "\n";
}

void Cpu::FuncPointerSetUp() {

    //set up function pointers for the main table
    table[0x0] = &Cpu::Table00;
    table[0x1] = &Cpu::OPcode_1NNN;
    table[0x2] = &Cpu::OPcode_2NNN;
    table[0x3] = &Cpu::OPcode_3XNN;
    table[0x4] = &Cpu::OPcode_4XNN;
    table[0x5] = &Cpu::OPcode_5XY0;
    table[0x6] = &Cpu::OPcode_6XNN;
    table[0x7] = &Cpu::OPcode_7XNN;
    table[0x8] = &Cpu::Table8;
    table[0x9] = &Cpu::OPcode_9XY0;
    table[0xA] = &Cpu::OPcode_ANNN;
    table[0xB] = &Cpu::OPcode_BNNN;
    table[0xC] = &Cpu::OPcode_CXNN;
    table[0xD] = &Cpu::OPcode_DXYN;
    table[0xE] = &Cpu::TableE;
    table[0xF] = &Cpu::TableF;

    //from main table gets indexed to sub-table 0
    table0[0x0] = &Cpu::OPcode_00E0;
    table0[0xE] = &Cpu::OPcode_00EE;

    //from main table gets indexed to sub-table 8
    table8[0x0] = &Cpu::OPcode_8XY0;
    table8[0x1] = &Cpu::OPcode_8XY1;
    table8[0x2] = &Cpu::OPcode_8XY2;
    table8[0x3] = &Cpu::OPcode_8XY3;
    table8[0x4] = &Cpu::OPcode_8XY4;
    table8[0x5] = &Cpu::OPcode_8XY5;
    table8[0x6] = &Cpu::OPcode_8XY6;
    table8[0x7] = &Cpu::OPcode_8XY7;
    table8[0xE] = &Cpu::OPcode_8XYE;

    //From main table gets indexed to sub-table E
    tableE[0x1] = &Cpu::OPcode_EXA1;
    tableE[0xE] = &Cpu::OPcode_EX9E;

    //from main table gets index to sub-table F
    tableF[0x07] = &Cpu::OPcode_FX07;
    tableF[0x0A] = &Cpu::OPcode_FX0A;
    tableF[0x15] = &Cpu::OPcode_FX15;
    tableF[0x18] = &Cpu::OPcode_FX18;
    tableF[0x1E] = &Cpu::OPcode_FX1E;
    tableF[0x29] = &Cpu::OPcode_FX29;
    tableF[0x33] = &Cpu::OPcode_FX33;
    tableF[0x55] = &Cpu::OPcode_FX55;
    tableF[0x65] = &Cpu::OPcode_FX65;

}

void Cpu::Table00() {
    (this->*(table0[ opcode & 0x000Fu ]))();
}

void Cpu::Table8() {
    (this->*(table8[ opcode & 0x000Fu ]))();
}

void Cpu::TableE() {
    (this->*(tableE[ opcode & 0x000Fu ]))();
}

void Cpu::TableF() {
    (this->*(tableF[ opcode & 0x00FFu ]))();
}

void Cpu::OPcode_00E0() {
    std::cout << "Clear the screen\n";
    //just memset buffer to 0
    memset(videoBufferPtr, 0, 64*32);
}

void Cpu::OPcode_00EE() {
    //decrease stack pointer to get previous programCounter
    std::cout << "decreasing stack ptr\n";
    stackIndex--;
    programCounter = stack[stackIndex];
}

void Cpu::OPcode_1NNN() {
    //"jumping" is just setting the program counter to the address in the opcode for next cycle
    std::cout << "jumping\n";
    programCounter = opcode & 0x0FFFu;
}

void Cpu::OPcode_2NNN() {
    std::cout << "store current program counter\n";
    //store current program counter so that it can be used to be returned to later
    stack[stackIndex] = programCounter;
    stackIndex++;
    //set next program counter to address in the opcode for next cycle
    programCounter = opcode & 0x0FFFu;
}

void Cpu::OPcode_3XNN() {
    std::cout << "skip the following instruction\n";
    if( registers[ (opcode & 0x0F00u) >> 8u ]  == (opcode & 0x00FFu) ){
        programCounter +=2;
    }
}

void Cpu::OPcode_4XNN() {
    std::cout << "skip the following instruction\n";
    if( registers[ (opcode & 0x0F00u) >> 8u ]  != (opcode & 0x00FFu) ){
        programCounter +=2;
    }
}

void Cpu::OPcode_5XY0() {
    std::cout << "skip the following instruction\n";
    if( registers[ (opcode & 0x0F00u) >> 8u ]  == registers[ (opcode & 0x00F0u) >> 4u ] ){
        programCounter +=2;
    }
}

void Cpu::OPcode_6XNN() {
    std::cout << "store number in register\n";
    registers[ (opcode & 0x0F00u) >> 8u ] = opcode & 0x00FFu;
}

void Cpu::OPcode_7XNN() {
    std::cout << "add number in register\n";
    registers[ (opcode & 0x0F00u) >> 8u ] += (opcode & 0x00FFu);
}

void Cpu::OPcode_8XY0() {
    std::cout << "Store the value of register VY in register VX  \n";
    registers[ (opcode & 0x0F00u) >> 8u ] = registers[ (opcode & 0x00F0u) >> 4u ];
}

void Cpu::OPcode_8XY1() {
    std::cout << "Set VX to VX OR VY \n";
    registers[ (opcode & 0x0F00u) >> 8u ] |= registers[ (opcode & 0x00F0u) >> 4u ];
}

void Cpu::OPcode_8XY2() {
    std::cout << "Set VX to VX AND VY \n";
    registers[ (opcode & 0x0F00u) >> 8u ] &= registers[ (opcode & 0x00F0u) >> 4u ];
}

void Cpu::OPcode_8XY3() {
    std::cout << "Set VX to VX XOR VY \n";
    registers[ (opcode & 0x0F00u) >> 8u ] ^= registers[ (opcode & 0x00F0u) >> 4u ];
}

void Cpu::OPcode_8XY4() {
    std::cout << "Add the value of register VY to register VX\n"
                 "Set VF to 01 if a carry occurs\n"
                 "Set VF to 00 if a carry does not occur\n";
    unsigned int sumRegXandY = registers[((opcode & 0x0F00u) >> 8u)] + registers[((opcode & 0x00F0u) >> 4u)];
    //test if value overflow
    (sumRegXandY) > 255 ? (registers[0xF] = 0x01) : (registers[0xF] = 0x00);
    //basically a sumReg%255
    registers[((opcode & 0x0F00u) >> 8u)] = sumRegXandY & 0xFF;
}

void Cpu::OPcode_8XY5() {
    std::cout << "Subtract the value of register VY from register VX\n"
                 "Set VF to 00 if a borrow occurs\n"
                 "Set VF to 01 if a borrow does not occur\n";
    //regX > register Y
    (registers[((opcode & 0x0F00u) >> 8u)] > registers[((opcode & 0x00F0u) >> 4u)]) ? (registers[0xF] = 0x00) : (registers[0xF] = 0x01);
    //regY = regY - regX
    registers[((opcode & 0x00F0u) >> 8u)] -= registers[((opcode & 0x0F00u) >> 4u)];
}

void Cpu::OPcode_8XY6() {
    std::cout << "Store the value of register VY shifted right one bit in register VX¹\n"
                 "Set register VF to the least significant bit prior to the shift\n"
                 "VY is unchanged\n";
    //get the least significant bit and store it in register VF
    registers[0xF] = registers[((opcode & 0x00F0u) >> 4u)] & 0x01;
    //shift value of regY 1 bit to the right and store in RegX
    registers[((opcode & 0x0F00u) >> 8u)] = (registers[((opcode & 0x00F0u) >> 4u)] >> 1u);
}

void Cpu::OPcode_8XY7() {
    std::cout << "Set register VX to the value of VY minus VX\n"
                 "Set VF to 00 if a borrow occurs\n"
                 "Set VF to 01 if a borrow does not occu\n";
    //reg X > reg Y
    (registers[((opcode & 0x0F00u) >> 8u)] > registers[((opcode & 0x00F0u) >> 4u)]) ? (registers[0xF] = 0x00) : (registers[0xF] = 0x01);
    //regX = regY - regX
    registers[((opcode & 0x0F00u) >> 8u)] = registers[((opcode & 0x00F0u) >> 4u)] - registers[((opcode & 0x0F00u) >> 8u)];
}

void Cpu::OPcode_8XYE() {
    std::cout << "Store the value of register VY shifted left one bit in register VX¹\n"
                 "Set register VF to the most significant bit prior to the shift\n"
                 "VY is unchanged\n";
    //get the most significant bit and store it in register VF
    registers[0xF] = (registers[((opcode & 0x00F0u) >> 4u)] & 0x80) >> 7u;
    //shift value of regY 1 bit to the left and store in RegX
    registers[((opcode & 0x0F00u) >> 8u)] = (registers[((opcode & 0x00F0u) >> 4u)] << 1u);
}

void Cpu::OPcode_9XY0() {
    std::cout << "Skip the following instruction if the value of register VX is not equal to the value of register VY \n";

    if(registers[((opcode & 0x0F00u) >> 8u)] != registers[((opcode & 0x00F0u) >> 4u)]){
        programCounter += 2;
    }
}

void Cpu::OPcode_ANNN() {
    std::cout << "Store memory address NNN in register I \n";
    indexRegister = opcode & 0x0FFFu;
}

void Cpu::OPcode_BNNN() {
    std::cout << "Jump to address NNN + V0 \n";
    programCounter = (opcode & 0x0FFFu) + registers[0];
}

void Cpu::OPcode_CXNN() {
    std::cout << "Set VX to a random number with a mask of NN \n";
    registers[(opcode & 0x0F00) >> 8u] = ((opcode & 0xFFu) & (rand() & 0xFFu) );
}

void Cpu::OPcode_DXYN() {
    std::cout << "Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I\n"
                 "Set VF to 01 if any set pixels are changed to unset, and 00 otherwise\n";

    unsigned int max = opcode & 0x000Fu;
    unsigned int xPosition = registers[(opcode & 0x0F00) >> 8u] & 0x3F;
    unsigned int yPosition = registers[(opcode & 0x00F0) >> 4u] & 0x1F;

    registers[0xFu] = 0x0;

    //might not work
    for(int i  = 0; i < max; i++){
        //get old sprite at location
        Byte spriteData = mainMemoryPtr[indexRegister + max];

        Byte oldVideoData = videoBufferPtr[ (xPosition) + ((yPosition + i) * 32) ];

        (videoBufferPtr[ (xPosition) + ((yPosition + i) * 32) ]) ^= spriteData;

        //if oldvideoData bytes and spritedata byte bitwise and to 0 that means that it detected no collision
        if((oldVideoData & spriteData) == 0x0){
            registers[0xFu] = 0x0;
        }
    }

    //update draw
    Emulator::updateDraw();

}

void Cpu::OPcode_EX9E() {
    std::cout << "Skip the following instruction if the key corresponding to the hex value currently stored in register VX is pressed\n";
    //skip next instruction if key is pressed
    int key = registers[opcode & 0x0F00 >> 8u] & 0xF;
    if (keyPadPtr[key] == 1) programCounter+=2;
}

void Cpu::OPcode_EXA1() {
    std::cout << "Skip the following instruction if the key corresponding to the hex value currently stored in register VX is not pressed\n";
    //skip next instruction if key is not pressed
    int key = registers[opcode & 0x0F00 >> 8u] & 0xF;
    if (keyPadPtr[key] == 0) programCounter+=2;
}

void Cpu::OPcode_FX07() {
    std::cout << "Store the current value of the delay timer in register VX \n";
    registers[(opcode & 0x0F00u) >> 8u] = delayTimer;
}

void Cpu::OPcode_FX0A() {
    std::cout << "Wait for a keypress and store the result in register VX \n";
    unsigned int registerKey = (opcode & 0x0F00u) >> 8u;

    for(int i = 0; i < KeyBoard::MAX_KEYPAD_SIZE; i++){
        if(keyPadPtr[i] == 1){
            registers[registerKey] = i;
            return;
        }
    }

    programCounter-=2;
}

void Cpu::OPcode_FX15() {
    std::cout << "Set the delay timer to the value of register VX \n";
    delayTimer = registers[(opcode & 0x0F00u) >> 8u];
}

void Cpu::OPcode_FX18() {
    std::cout << "Set the sound timer to the value of register VX \n";
    soundTimer = registers[(opcode & 0x0F00u) >> 8u];
}

void Cpu::OPcode_FX1E() {
    std::cout << "Add the value stored in register VX to register I \n";
    indexRegister += registers[(opcode & 0x0F00u)];
}

void Cpu::OPcode_FX29() {
    std::cout << "Set I to the memory address of the sprite data corresponding to the hexadecimal digit stored in register VX \n";
    indexRegister = 0x50 + (5 * registers[(opcode & 0x0F00) >> 8u]);
}

void Cpu::OPcode_FX33() {
    std::cout << "Store the binary-coded decimal equivalent of the value stored in register VX at addresses I, I + 1, and I + 2 \n";
    Byte value = registers[ (opcode & 0x0F00u) >> 8u ];

    mainMemoryPtr[indexRegister] = (value/100);
    mainMemoryPtr[indexRegister+ 1] = (value/100) % 10;
    mainMemoryPtr[indexRegister+ 2] = (value%100) % 10;
}

void Cpu::OPcode_FX55() {
    std::cout << "Store the values of registers V0 to VX inclusive in memory starting at address I\n"
                 "I is set to I + X + 1 after operation²\n";
    Byte maxLength = (opcode & 0x0F00u) >> 8u;

    for(int i = 0; i <= maxLength; ++i){
        mainMemoryPtr[indexRegister + i] = registers[i];
    }

    indexRegister += (1 + maxLength);
}

void Cpu::OPcode_FX65() {
    std::cout << "Fill registers V0 to VX inclusive with the values stored in memory starting at address I\n"
                 "I is set to I + X + 1 after operation²\n";
    Byte maxLength = (opcode & 0x0F00u) >> 8u;

    for(int i = 0; i <= maxLength; ++i){
        registers[i] = mainMemoryPtr[indexRegister + i];
    }

    indexRegister += (1 + maxLength);
}










































