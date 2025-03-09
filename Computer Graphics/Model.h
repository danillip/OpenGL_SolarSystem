#ifndef MODEL_H
#define MODEL_H

#include <string>
#include <vector>
#include <glm.hpp>
#include "Texture.h"
#include <GL/glew.h>

// Структура вершины: позиция, нормаль и текстурные координаты
struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

class Model {
public:
    Model(const std::string& path);
    ~Model();
    void Draw();

    // Если материал содержит диффузную текстуру, она будет загружена сюда
    Texture* diffuseTexture;

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;

    bool loadModel(const std::string& path);
    void setupMesh();
};

#endif
