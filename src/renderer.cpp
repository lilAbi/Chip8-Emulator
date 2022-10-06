
#include "renderer.h"

static const float quadBuffer[] = {
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};

void Renderer::Init() {
    //load the shader program
    shader.LoadShaderProgram("/Users/abi/CLionProjects/CHIP8Emulator/src/resource/emu.vert","/Users/abi/CLionProjects/CHIP8Emulator/src/resource/emu.frag");

    //clear texture data
    std::memset(&textureData, 0, sizeof(textureData));

    //generate vao and bind
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    //generate vbo and bind
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    //send vertex data to gpu
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadBuffer), quadBuffer, GL_STATIC_DRAW);
    //inform gpu of position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //inform gpu of coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);


    //set up and texture and add data
    renderTexture = Texture(32, 64);
    renderTexture.SetData((void*)textureData);

    //activate shader
    shader.Use();
    shader.SetInt("texture1", 0);
}

void Renderer::Draw() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


   // glViewport(0, 0, Window::windowWidth, Window::windowHeight);

    glActiveTexture(GL_TEXTURE0);
    renderTexture.BindTexture();

    shader.Use();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::Update(Byte *drawBuffer) {

    std::cout << "updating draw\n";

    for(int h = 0; h < 31; h++){
        for(int w = 0; w < 63; w++){
            if( auto pixel = drawBuffer[w + (h*64)]; pixel == 0){
                textureData[ (w*3) + (h*64*3) ]       = 0;
                textureData[ (w*3) + (h*64*3) + 1]    = 0;
                textureData[ (w*3) + (h*64*3) + 2]    = 0;
            } else {
                textureData[ (w*3) + (h*64*3) ]       = 255;
                textureData[ (w*3) + (h*64*3) + 1]    = 255;
                textureData[ (w*3) + (h*64*3) + 2]    = 255;
            }
        }
    }

    renderTexture.SetData((void*)textureData);
}

