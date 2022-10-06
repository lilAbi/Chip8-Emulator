#include <iostream>
#include "src/emulator.h"


int main() {
    //construct emulator object
    Emulator emulator;

    //initialize emulator
    emulator.Init();

    //load rom into emulator;
    emulator.LoadRom("/Users/abi/Desktop/random_number_test.ch8");

    //run emulator
    emulator.Run();

    return 0;
}
