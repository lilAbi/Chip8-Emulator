
#include "texture.h"

Texture::Texture(int textureWidth, int textureHeight) : width{textureWidth}, height{textureHeight} {
    //generate texture ID and bind that bad girl
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    //set up texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

    //create texture storage on gpu
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

}

Texture::~Texture() {
    glDeleteTextures(1, &textureID);
}

Texture& Texture::operator=(const Texture &rhs) {
    width       = rhs.width;
    height      = rhs.height;
    textureID   = rhs.textureID;

    return *this;
}


void Texture::SetData(void *data) {
    BindTexture();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::BindTexture() {
    glBindTexture(GL_TEXTURE_2D, textureID);
}

