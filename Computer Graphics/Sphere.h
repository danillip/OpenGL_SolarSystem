#ifndef SPHERE_H
#define SPHERE_H

#include <vector>
#include <GL/glew.h>
#include <glm.hpp>

class Sphere {
public:
    Sphere(float radius, unsigned int sectors, unsigned int stacks);
    ~Sphere();
    void draw();
private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO, VBO, EBO;
    void buildSphere(float radius, unsigned int sectors, unsigned int stacks);
};

#endif
