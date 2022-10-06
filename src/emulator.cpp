
#include "emulator.h"

//runs the
void Emulator::Run() {

    while(!glfwWindowShouldClose(Window::windowPtr)){

        //pool input
        PollInput();
        //run cpu
        cpu.ExecuteCPU();

        //draw
        if(updateNewDrawFlag) {
            renderer.Update(videoBuffer);
            updateNewDrawFlag = false;
        }

        renderer.Draw();

        glfwSwapBuffers(Window::windowPtr);
        glfwPollEvents();
    }

}

void Emulator::Init() {
    //initialize CPU sub-features
    cpu.Init(mainMemory, videoBuffer, keyMap);

    //memset mainMemory and videoMemory to 0
    memset(mainMemory, 0, 4096);
    memset(videoBuffer, 0, 64*32);

    //initialize GLFW
    window.Init();

    //initialize renderer
    renderer.Init();

}

void Emulator::LoadRom(const char *filename) {
    //open file
    std::fstream filePtr(filename);
    //get size of rom file
    auto fileSize = std::filesystem::file_size(std::filesystem::path(filename));
    //load rom file contents into memory
    filePtr.read( reinterpret_cast<char*>(&mainMemory[0x200u]) , static_cast<long>(fileSize));
    //close file
    filePtr.close();
}

void Emulator::PollInput() {
    //shut down window;
    if (glfwGetKey(Window::windowPtr, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(Window::windowPtr, true);

    //clear input buffer
    std::memset(&keyMap, 0, KeyBoard::MAX_KEYPAD_SIZE);

    //look for key press
    if (glfwGetKey(Window::windowPtr, GLFW_KEY_1) == GLFW_PRESS) {
        keyMap[KEY_1] = 1;

    } else if (glfwGetKey(Window::windowPtr, GLFW_KEY_2) == GLFW_PRESS){
        keyMap[KEY_2] = 1;

    } else if (glfwGetKey(Window::windowPtr, GLFW_KEY_3) == GLFW_PRESS){
        keyMap[KEY_3] = 1;

    } else if (glfwGetKey(Window::windowPtr, GLFW_KEY_4) == GLFW_PRESS){
        keyMap[KEY_4] = 1;

    } else if (glfwGetKey(Window::windowPtr, GLFW_KEY_Q) == GLFW_PRESS){
        keyMap[KEY_Q] = 1;

    } else if (glfwGetKey(Window::windowPtr, GLFW_KEY_W) == GLFW_PRESS){
        keyMap[KEY_W] = 1;

    } else if (glfwGetKey(Window::windowPtr, GLFW_KEY_E) == GLFW_PRESS){
        keyMap[KEY_E] = 1;

    } else if (glfwGetKey(Window::windowPtr, GLFW_KEY_R) == GLFW_PRESS){
        keyMap[KEY_R] = 1;

    } else if (glfwGetKey(Window::windowPtr, GLFW_KEY_A) == GLFW_PRESS){
        keyMap[KEY_A] = 1;

    } else if (glfwGetKey(Window::windowPtr, GLFW_KEY_S) == GLFW_PRESS){
        keyMap[KEY_S] = 1;

    } else if (glfwGetKey(Window::windowPtr, GLFW_KEY_D) == GLFW_PRESS){
        keyMap[KEY_D] = 1;

    } else if (glfwGetKey(Window::windowPtr, GLFW_KEY_F) == GLFW_PRESS){
        keyMap[KEY_F] = 1;

    } else if (glfwGetKey(Window::windowPtr, GLFW_KEY_Z) == GLFW_PRESS){
        keyMap[KEY_Z] = 1;

    } else if (glfwGetKey(Window::windowPtr, GLFW_KEY_X) == GLFW_PRESS){
        keyMap[KEY_X] = 1;

    } else if (glfwGetKey(Window::windowPtr, GLFW_KEY_C) == GLFW_PRESS){
        keyMap[KEY_C] = 1;

    } else if (glfwGetKey(Window::windowPtr, GLFW_KEY_V) == GLFW_PRESS){
        keyMap[KEY_V] = 1;
    }

}
