cmake_minimum_required(VERSION 3.23)

project(CHIP8Emulator)

set(CMAKE_CXX_STANDARD 20)
include_directories(src)
include_directories(include)

find_package(glfw3 3.3 REQUIRED)

file(GLOB SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")

link_directories(/usr/local/Cellar/llvm/14.0.6_1/lib)

add_executable(CHIP8Emulator main.cpp glad.c ${SRC_FILES})
target_link_libraries(CHIP8Emulator glfw)
