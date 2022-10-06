
#ifndef CHIP8EMULATOR_RENDERER_H
#define CHIP8EMULATOR_RENDERER_H

#include "window.h"
#include "shader.h"
#include "texture.h"

using Byte = uint8_t;

class Renderer {
public:
    //initialize
    void Init();
    //draw
    void Draw();
    //updates the drawing buffer
    void Update(Byte *videoBuffer);

public:
    //shader
    Shader shader;
    //render texture
    Texture renderTexture;

    unsigned int vao{};
    unsigned int vbo{};

    Byte textureData[64 * 32 * 3]{};

};


#endif //CHIP8EMULATOR_RENDERER_H
