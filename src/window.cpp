
#include "window.h"

void Window::Init() {
    //configure opengl
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    windowPtr = glfwCreateWindow(windowWidth, windowHeight, "Chip8 Emulator", nullptr, nullptr);
    if (windowPtr == nullptr){
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(windowPtr);

    glfwSetInputMode(windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return;
    }

    glEnable(GL_DEPTH_TEST);
}
