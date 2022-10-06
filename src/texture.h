
#ifndef CHIP8EMULATOR_TEXTURE_H
#define CHIP8EMULATOR_TEXTURE_H

#include "glad/glad.h"


//build a texture from a color buffer
class Texture {
public:
    Texture() = default;
    Texture(int width, int height);
    //clean up texture
    ~Texture();

    //copy assignment
    Texture& operator=(const Texture &rhs);

    //getter methods for texture properties
    unsigned int GetWidth() const { return width; }
    unsigned int GetHeight() const { return height; }
    unsigned int GetTextureID() const { return textureID; }

    //upload data to set
    void SetData(void* data);

    //bind texture
    void BindTexture();

private:
    int width{};
    int height{};
    unsigned int textureID{};
};


#endif //CHIP8EMULATOR_TEXTURE_H
