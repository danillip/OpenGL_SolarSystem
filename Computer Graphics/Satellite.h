#ifndef SATELLITE_H
#define SATELLITE_H

#include <string>
#include "Sphere.h"
class Planet;  // Предварительное объявление класса Planet
class Shader;  // Предварительное объявление класса Shader

#include <glm.hpp>

class Satellite {
public:
    Satellite(const std::string& name, float radius, float orbitRadius, float orbitSpeed, float rotationSpeed, Planet* parentPlanet);
    void draw(Shader& shader, float simulationTime, const glm::mat4& parentTransform);

private:
    std::string name;
    float radius;
    float orbitRadius;
    float orbitSpeed;
    float rotationSpeed;
    Planet* parentPlanet;
    Sphere* sphere;
};

#endif