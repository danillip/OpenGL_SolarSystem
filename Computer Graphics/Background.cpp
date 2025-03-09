#include "Background.h"
#include <cstdio>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

Background::Background(const char* directory, int numFrames, float frameDelay)
    : numFrames(numFrames), frameDelay(frameDelay)
{
    char filename[256];
    for (int i = 0; i < numFrames; i++) {
        // sprintf_s для безопасности
        sprintf_s(filename, sizeof(filename), "%s/frame_%02d_delay-0.05s.jpg", directory, i);
        Texture* tex = new Texture(filename);
        textures.push_back(tex);
    }

    // Создание VAO для полноэкранного квадрата
    float quadVertices[] = {
        // позиции   // текстурные координаты
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);
}

Background::~Background() {
    for (Texture* tex : textures)
        delete tex;
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
}

void Background::Draw(Shader& shader) {
    int frameIndex = (int)(glfwGetTime() / frameDelay) % numFrames;
    shader.use();
    glActiveTexture(GL_TEXTURE0);
    textures[frameIndex]->Bind();
    shader.setInt("backgroundTexture", 0);

    // Отключаем тест глубины чтобы фон отображался позади сцены
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
}
