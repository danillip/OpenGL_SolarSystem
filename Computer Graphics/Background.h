#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <vector>
#include "Texture.h"
#include "Shader.h"

class Background {
public:
    // directory Ц путь к папке с кадрами, numFrames Ц количество кадров, frameDelay Ц задержка (в секундах) на один кадр
    Background(const char* directory, int numFrames, float frameDelay);
    ~Background();
    void Draw(Shader& shader);
private:
    std::vector<Texture*> textures;
    unsigned int quadVAO, quadVBO;
    int numFrames;
    float frameDelay;
};

#endif
