#pragma once
#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include "Shader.h"
#include "Sphere.h"
#include <vector>
#include <glm.hpp>

class Asteroids {
public:
    Asteroids();
    ~Asteroids();
    void init();
    void draw(Shader& shader, float simulationTime);
private:
    std::vector<glm::vec3> positions;
    Sphere* asteroidSphere;
};

#endif
