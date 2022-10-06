#ifndef CHIP8EMULATOR_WINDOW_H
#define CHIP8EMULATOR_WINDOW_H

#include "glad/glad.h"
#include "GLFW/glfw3.h"

class Window {
public:
    //initialize GLFW
    void Init();

public:
    inline static int windowHeight{320};
    inline static int windowWidth{640};
    inline static GLFWwindow* windowPtr;
};


#endif //CHIP8EMULATOR_WINDOW_H
