#include "Satellite.h"
#include "Shader.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>

Satellite::Satellite(const std::string& name, float radius, float orbitRadius, float orbitSpeed, float rotationSpeed, Planet* parentPlanet)
    : name(name), radius(radius), orbitRadius(orbitRadius), orbitSpeed(orbitSpeed), rotationSpeed(rotationSpeed), parentPlanet(parentPlanet)
{
    sphere = new Sphere(radius, 36, 18);  // Создаём сферу для спутника
}

void Satellite::draw(Shader& shader, float simulationTime, const glm::mat4& parentTransform) {
    glm::mat4 modelMatrix = parentTransform;

    // Вращение спутника вокруг своей планеты
    float angleOrbit = simulationTime * orbitSpeed;
    modelMatrix = glm::rotate(modelMatrix, glm::radians(angleOrbit), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(orbitRadius, 0.0f, 0.0f));

    // Вращение спутника вокруг своей оси
    float angleSelf = simulationTime * rotationSpeed;
    modelMatrix = glm::rotate(modelMatrix, glm::radians(angleSelf), glm::vec3(0.0f, 1.0f, 0.0f));

    shader.setMat4("model", modelMatrix);
    shader.setInt("useTexture", 0);               // Отключаем текстуру для спутника
    shader.setVec3("objectColor", glm::vec3(0.8f)); // Однородный светлый цвет, можно настроить
    sphere->draw();
}
